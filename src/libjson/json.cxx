#include "libjson/json.hxx"

namespace libjson {
/**
 * @brief Trim left string
 *
 * @param str The string to trim
 */
auto ltrim(std::string str) noexcept -> std::string;

// ----------------------------------------------------------------------------

auto ltrim(std::string str) noexcept -> std::string {
    str.erase(
        str.begin(), std::find_if(str.begin(), str.end(),
                         [](const auto ch) { return !std::isspace(ch); }));
    return str;
}

json::json(const json& rhs)
    : _hasValue(rhs._hasValue),
      _value(_deep_copy(rhs._value)) {}

auto json::operator =(const json& rhs) -> json& {
    if (this != &rhs) {
        _hasValue = rhs._hasValue;
        _value    = _deep_copy(rhs._value);
    }

    return *this;
}

json::json(json&& rhs)
    : _hasValue(rhs._hasValue),
      _value(std::move(rhs._value)) {
    rhs._hasValue = false;
}

auto json::operator =(json&& rhs) -> json& {
    _hasValue     = rhs._hasValue;
    _value        = std::move(rhs._value);
    rhs._hasValue = false;
    return *this;
}

auto json::size(void) const noexcept -> std::size_t {
    if (!_hasValue) return 0;
    return std::visit(
        [](const auto& val) -> bool {
            using clean_t = std::remove_cvref_t<decltype(val)>;

            if constexpr (std::same_as<clean_t, object_ptr_t>
                          || std::same_as<clean_t, array_ptr_t>)
                return val->size();
            return 1;
        },
        _value);
}

auto json::has_value(void) const noexcept -> bool {
    return _hasValue;
}


auto json::at(std::size_t index) const -> const json& {
    if (!_hasValue) throw std::runtime_error("No value is being stored");
    return std::get<array_ptr_t>(_value)->at(index);
}

auto json::at(std::string_view key) const -> const json& {
    if (!_hasValue) throw std::runtime_error("No value is being stored");
    return std::get<object_ptr_t>(_value)->at(std::string {key});
}

auto json::operator [](std::string_view key) -> json& {
    if (!_hasValue) {
        _hasValue = true;
        _value    = std::make_unique<object_t>(object_t {});
    }

    return (*std::get<object_ptr_t>(_value))[std::string {key}];
}

auto json::operator [](std::size_t index) -> json& {
    if (!_hasValue) throw std::runtime_error("No value is being stored");
    return std::get<array_ptr_t>(_value)->at(index);
}

auto json::push_back(const json& node) -> void {
    if (!_hasValue) {
        _hasValue = true;
        _value    = std::make_unique<array_t>(array_t {});
    }

    if (!holds_alternative<array_t>()) {
        auto buffer {std::visit(
            [](auto& node) -> json {
                using Tp = decltype(node);

                if constexpr (requires(Tp x) { *x; })
                    return {*node};
                else
                    return {node};
            },
            _value)};

        _value = std::make_unique<array_t>(array_t {std::move(buffer)});
    }

    std::get<array_ptr_t>(_value)->push_back(node);
}

auto json::push_back(json&& node) -> void {
    if (!_hasValue) {
        _hasValue = true;
        _value    = std::make_unique<array_t>(array_t {});
    }

    if (!holds_alternative<array_t>()) {
        auto buffer {std::visit(
            [](auto& node) -> json {
                using Tp = decltype(node);

                if constexpr (requires(Tp x) { *x; })
                    return {*node};
                else
                    return {node};
            },
            _value)};

        _value = std::make_unique<array_t>(array_t {std::move(buffer)});
    }

    std::get<array_ptr_t>(_value)->push_back(std::move(node));
}

auto json::insert(std::string_view key, const json& node) -> void {
    if (!_hasValue) {
        _hasValue = true;
        _value    = std::make_unique<object_t>(object_t {});
    }

    const std::string str {key};
    auto& ptr {std::get<object_ptr_t>(_value)};

    if (ptr->contains(str))
        throw std::runtime_error(
            std::format("Key already present in object: {}", key));
    ptr->insert({str, node});
}

auto json::insert(std::string_view key, json&& node) -> void {
    if (!_hasValue) {
        _hasValue = true;
        _value    = std::make_unique<object_t>(object_t {});
    }

    const std::string str {key};
    auto& ptr {std::get<object_ptr_t>(_value)};

    if (ptr->contains(str))
        throw std::runtime_error(
            std::format("Key already present in object: {}", key));
    ptr->insert({str, std::move(node)});
}

auto json::encode(std::size_t indent) const -> std::string {
    return _encode_recursive(*this, indent, 0);
}

auto operator <<(std::ostream& os, const json& node) -> std::ostream& {
    os << node.encode(os.width());
    os.width(0);

    return os;
}

auto json::_deep_copy(const value_t_internal& val) noexcept
    -> value_t_internal {
    return std::visit(
        [](const auto& v) noexcept -> value_t_internal {
            using clean_t = std::remove_cvref_t<decltype(v)>;

            if constexpr (requires(clean_t x) { *x; }) {
                using element_t = typename clean_t::element_type;
                using target_t  = std::conditional_t<
                    std::constructible_from<std::string, element_t>,
                    std::string, element_t>;

                return std::make_unique<target_t>(*v);
            } else
                return v;
        },
        val);
};

auto json::_encode_recursive(
    const json& node, std::size_t indent, std::size_t depth) -> std::string {
    if (depth == MAX_DEPTH)
        throw std::runtime_error("Max depth reached for encoding.");

    return std::visit(
        [&](const auto& v) noexcept -> std::string {
            using clean_t = std::remove_cvref_t<decltype(v)>;

            std::string buffer {};

            if (indent > 0) buffer += std::format("{:{}}", "", indent * depth);

            if (!node._hasValue) {
                buffer += "null";
                return buffer;
            }

            if constexpr (requires(clean_t x) { *x; }) {
                if constexpr (std::same_as<clean_t, array_ptr_t>) {
                    buffer += '[';

                    if (indent > 0) buffer += '\n';

                    const auto sz {v->size()};

                    for (std::size_t i {}; i < sz; i++) {
                        buffer
                            += _encode_recursive(v->at(i), indent, depth + 1);

                        if (i != sz - 1) {
                            buffer += ",";
                            if (indent > 0) buffer += '\n';
                        }
                    }

                    if (indent > 0)
                        buffer += std::format("\n{:{}}", "", indent * depth);

                    buffer += ']';
                } else if constexpr (std::same_as<clean_t, object_ptr_t>) {
                    buffer += '{';

                    if (indent > 0) buffer += '\n';

                    const auto sz {v->size()};
                    std::size_t i {};

                    std::vector<std::string> keys;
                    for (const auto& [k, _] : *v) keys.push_back(k);

                    std::sort(keys.begin(), keys.end());

                    for (const auto& k : keys) {
                        if (indent > 0)
                            buffer += std::format(
                                "{:{}}", "", indent * (depth + 1));

                        buffer += std::format("{:?}: ", k)
                                  + ltrim(_encode_recursive(
                                      v->at(k), indent, depth + 1));

                        if (i != sz - 1) {
                            buffer += ",";
                            if (indent > 0) buffer += '\n';
                        }

                        i++;
                    }

                    if (indent > 0)
                        buffer += std::format("\n{:{}}", "", indent * depth);

                    buffer += '}';
                } else {
                    buffer += '"';

                    for (const auto& ch : *v) {
                        switch (ch) {
                            case '"':
                            case '\\':
                            case '/':
                            case '\b':
                            case '\f':
                            case '\n':
                            case '\r':
                            case '\t':
                                buffer += '\\' + ch;
                                break;
                            default:
                                if (ch < 0x20) {
                                    static constexpr char hex_digits[]
                                        = "0123456789ABCDEF";
                                    buffer += "\\u00";
                                    buffer += hex_digits[(ch >> 4) & 0x0F];
                                    buffer += hex_digits[ch & 0x0F];
                                } else
                                    buffer += ch;
                        }
                    }

                    buffer += '"';
                }
            } else if constexpr (std::same_as<clean_t, bool>)
                buffer += v ? "true" : "false";
            else
                buffer += std::to_string(v);

            return buffer;
        },
        node._value);
}
} // namespace libjson
