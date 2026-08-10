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

auto json::has_value(void) const noexcept -> bool {
    return _hasValue;
}

auto json::encode(std::size_t indent) const -> std::string {
    return _encode_recursive(*this, indent, 0);
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
                            buffer += ", ";
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

                    for (const auto& [k, n] : *v) {
                        if (indent > 0)
                            buffer += std::format(
                                "{:{}}", "", indent * (depth + 1));

                        buffer
                            += std::format("{:?}: ", k)
                               + ltrim(_encode_recursive(n, indent, depth + 1));

                        if (i != sz - 1) {
                            buffer += ", ";
                            if (indent > 0) buffer += '\n';
                        }

                        i++;
                    }

                    if (indent > 0)
                        buffer += std::format("\n{:{}}", "", indent * depth);

                    buffer += '}';
                } else {
                    // TODO: encode string
                    buffer += '"' + *v + '"';
                }
            } else
                buffer += std::to_string(v);

            return buffer;
        },
        node._value);
}
} // namespace libjson
