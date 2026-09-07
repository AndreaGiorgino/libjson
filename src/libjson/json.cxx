#include <cassert>
#include <functional>
#include <libjson/json.hxx>
#include <libjson/parse_error.hxx>
#include <sstream>

namespace libjson {
/**
 * @brief Trim left string
 *
 * @param str The string to trim
 */
auto ltrim(std::string str) noexcept -> std::string;

/**
 * @brief Throw if EOF is encountered
 *
 * @param is
 *
 * @throws parse_error If EOF is encountered
 */
auto throw_eof(std::istream& is) -> void;

/**
 * @brief Skip whitespaces in stream
 *
 * @param is The input stream
 */
auto skipws(std::istream& is) noexcept -> void;

/**
 * @brief Append utf8 character to string
 *
 * @param codePoint The character to append
 * @param out The string to append to
 */
void append_utf8(char32_t codePoint, std::string& out);

/**
 * @brief Get characters until the stop condition is met
 *
 * @param is The input stream
 * @param stop_condition
 * @return The string formed by the consumed characters
 */
[[nodiscard]] auto get_until(std::istream& is,
    std::function<bool(char)> stop_condition) noexcept -> std::string;

/**
 * @brief Parse from stream a json value
 *
 * @param is The input stream
 *
 * @throws parse_error If the stream is malformed
 */
[[nodiscard]] auto parse(std::istream& is) -> json;

/**
 * @brief Parse from stream a json object value
 *
 * @param is The input stream
 * @return A json containing the parsed object value
 *
 * @throws parse_error If a premature EOF is encountered
 * @throws parse_error If the object is unclosed
 */
[[nodiscard]] auto parse_object(std::istream& is) -> json;

/**
 * @brief Parse from stream a json array value
 *
 * @param is The input stream
 * @return A json containing the parsed array value
 *
 * @throws parse_error If a premature EOF is encountered
 * @throws parse_error If a trailing comma is encountered
 * @throws parse_error If an unexpected character is encountered
 * @throws parse_error If the array is unclosed
 */
[[nodiscard]] auto parse_array(std::istream& is) -> json;

/**
 * @brief Parse from stream a json primitive value (bool/int/double/string)
 *
 * @param is The input stream
 * @return A json containing the parsed primitive value
 *
 * @throws parse_error If a premature EOF is encountered
 * @throws parse_error If the string is unclosed
 * @throws parse_error If the number is Invalid
 * @throws parse_error If an unexpected character is encountered
 * @throws parse_error If an unexpected literal is encountered
 */
[[nodiscard]] auto parse_value(std::istream& is) -> json;

// ----------------------------------------------------------------------------

auto ltrim(std::string str) noexcept -> std::string {
    str.erase(
        str.begin(), std::find_if(str.begin(), str.end(),
                         [](const auto ch) { return !std::isspace(ch); }));
    return str;
}

auto throw_eof(std::istream& is) -> void {
    if (is.eof()) throw parse_error("Premature EOF encountered");
}

auto skipws(std::istream& is) noexcept -> void {
    while (!is.eof() && std::isspace(is.peek())) is.ignore();
}

void append_utf8(char32_t codePoint, std::string& out) {
    if (codePoint <= 0x7F) {
        out.push_back((char)(codePoint));
    } else if (codePoint <= 0x7FF) {
        out.push_back((char)(0xC0 | ((codePoint >> 6) & 0x1F)));
        out.push_back((char)(0x80 | (codePoint & 0x3F)));
    } else if (codePoint <= 0xFFFF) {
        out.push_back((char)(0xE0 | ((codePoint >> 12) & 0x0F)));
        out.push_back((char)(0x80 | ((codePoint >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (codePoint & 0x3F)));
    } else if (codePoint <= 0x10FFFF) {
        out.push_back((char)(0xF0 | ((codePoint >> 18) & 0x07)));
        out.push_back((char)(0x80 | ((codePoint >> 12) & 0x3F)));
        out.push_back((char)(0x80 | ((codePoint >> 6) & 0x3F)));
        out.push_back((char)(0x80 | (codePoint & 0x3F)));
    }
}

auto get_until(std::istream& is,
    std::function<bool(char)> stop_condition) noexcept -> std::string {
    if (is.eof()) return {};

    std::string buffer {};
    while (!is.eof() && !stop_condition(is.peek())) buffer += is.get();

    return buffer;
}

auto parse_object(std::istream& is) -> json {
    throw_eof(is);
    assert(is.peek() == '{');

    const auto start {is.tellg()};
    is.ignore();

    object_t buffer {};
    while (!is.eof()) {
        skipws(is);
        throw_eof(is);

        if (is.peek() == '}') {
            if (buffer.size() == 0) break;
            throw parse_error(
                std::format("Trailing comma encountered at position {}",
                    (std::size_t)is.tellg() + 1));
        }

        const auto key {parse(is)};
        if (!key.holds_alternative<std::string>())
            throw parse_error(
                std::format("Expected a string as key in object at position {}",
                    (std::size_t)start + 1));

        skipws(is);
        throw_eof(is);

        if (is.peek() != ':')
            throw parse_error(std::format(
                "Expected a colon between a key-value pair at position {}: {}",
                (std::size_t)is.tellg(), is.peek()));

        is.ignore();
        skipws(is);
        throw_eof(is);

        const auto node {parse(is)};
        buffer[key.as<std::string>()] = std::move(node);

        skipws(is);
        throw_eof(is);

        const auto ch {is.peek()};
        if (ch == '}')
            break;
        else if (ch == ',') {
            is.ignore();
            continue;
        }

        throw parse_error(std::format("Unexpected character at position {}: {}",
            (std::size_t)is.tellg() + 1, (char)ch));
    }

    if (is.eof() || is.peek() != '}')
        throw parse_error(std::format(
            "Unclosed object opened at position {}", (std::size_t)start + 1));

    is.ignore();
    return buffer;
}

auto parse_array(std::istream& is) -> json {
    throw_eof(is);
    assert(is.peek() == '[');

    const auto start {is.tellg()};
    is.ignore();

    array_t buffer {};
    while (!is.eof()) {
        skipws(is);
        throw_eof(is);

        if (is.peek() == ']') {
            if (buffer.size() == 0) break;
            throw parse_error(
                std::format("Trailing comma encountered at position {}",
                    (std::size_t)is.tellg() + 1));
        }

        buffer.push_back(parse(is));

        skipws(is);
        throw_eof(is);

        const auto ch {is.peek()};
        if (ch == ']')
            break;
        else if (ch == ',') {
            is.ignore();
            continue;
        }

        throw parse_error(std::format("Unexpected character at position {}: {}",
            (std::size_t)is.tellg() + 1, (char)ch));
    }

    if (is.eof() || is.peek() != ']')
        throw parse_error(std::format(
            "Unclosed array opened at position {}", (std::size_t)start + 1));

    is.ignore();
    return buffer;
}

auto parse_value(std::istream& is) -> json {
    throw_eof(is);

    const auto start {is.tellg()};
    const auto ch {is.peek()};

    if (ch == '"') {
        is.ignore();
        std::string buffer {};

        while (!is.eof()) {
            buffer += get_until(is, [](const auto ch) {
                switch (ch) {
                    case '"':
                    case '\n':
                    case '\\':
                        return true;
                    default:
                        return false;
                }
            });

            if (is.peek() != '\\') break;
            is.ignore();

            // handle escaped characters
            if (is.eof()) break;

            switch (is.peek()) {
                case '"':
                case '\\':
                case '/':
                    buffer += is.get();
                    break;
                case 'b': // backspace
                    is.ignore();

                    buffer += '\b';
                    break;
                case 'f': // form feed
                    is.ignore();

                    buffer += '\f';
                    break;
                case 'n': // newline
                    is.ignore();

                    buffer += '\n';
                    break;
                case 'r': // carriage return
                    is.ignore();

                    buffer += '\r';
                    break;
                case 't': // tab
                    is.ignore();

                    buffer += '\t';
                    break;
                case 'u': { // unicode
                    is.ignore();

                    std::string hex {};
                    for (int i {}; i < 4; i++) {
                        if (is.eof())
                            throw parse_error(std::format(
                                "Incomplete unicode character at position {}",
                                (std::size_t)is.tellg() + i + 1));

                        hex += is.get();
                    }

                    try {
                        const auto codePoint {
                            (uint16_t)std::stoul(hex, NULL, 16)};
                        append_utf8(codePoint, buffer);
                    } catch (...) {
                        throw parse_error(std::format(
                            "Invalid unicode sequence at position {}: \\u{}",
                            (std::size_t)is.tellg() - 3, hex));
                    }

                    break;
                }
                default:
                    throw parse_error(std::format(
                        "Invalid escape sequence at position {}: \\{}",
                        (std::size_t)is.tellg(), (char)is.peek()));
            }
        }

        if (is.eof() || is.peek() != '"')
            throw parse_error(
                std::format("Unclosed string opened at position {}: {}",
                    (std::size_t)start + 1, buffer));

        is.ignore();
        return buffer;
    } else if (ch == '-' || std::isdigit(ch)) {
        std::string buffer {get_until(is, [](const auto ch) {
            return ch != '.' && ch != '-' && !std::isdigit(ch);
        })};

        try {
            if (buffer.find('.') != std::string::npos) return std::stod(buffer);
            return std::stoi(buffer);
        } catch (...) {
            throw parse_error(
                std::format("Cannot parse number at position {}: {}",
                    (std::size_t)start + 1, buffer));
        }
    } else if (std::isalpha(ch)) {
        std::string buffer {
            get_until(is, [](const auto ch) { return !std::isalpha(ch); })};

        if (buffer == "null")
            return {};
        else if (buffer == "false")
            return false;
        else if (buffer == "true")
            return true;

        throw parse_error(std::format("Unexpected literal at position {}: {}",
            (std::size_t)start + 1, buffer));
    }

    throw parse_error(std::format("Unexpected character at position {}: {}",
        (std::size_t)start + 1, (char)ch));
}

auto parse(std::istream& is) -> json {
    skipws(is);
    throw_eof(is);

    if (is.peek() == '{')
        return parse_object(is);
    else if (is.peek() == '[')
        return parse_array(is);
    return parse_value(is);
}

auto json::decode(std::istream& is) -> json {
    if (!is) throw std::runtime_error("Invalid stream provided");

    skipws(is);
    if (is.eof()) return {};

    const auto ret {parse(is)};

    skipws(is);
    if (!is.eof()) throw parse_error("Single value expected at root level");

    return ret;
}

auto json::decode(std::string_view raw) -> json {
    std::istringstream ss {raw.data()};
    return decode(ss);
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

auto json::empty(void) const noexcept -> bool {
    if (!_hasValue) return true;
    return std::visit(
        [](const auto& val) -> std::size_t {
            using clean_t = std::remove_cvref_t<decltype(val)>;

            if constexpr (std::same_as<clean_t, object_ptr_t>
                          || std::same_as<clean_t, array_ptr_t>)
                return val->empty();
            return false;
        },
        _value);
}

auto json::has_value(void) const noexcept -> bool {
    return _hasValue;
}

auto json::contains(std::string_view key) const noexcept -> bool {
    if (!_hasValue) return false;
    return std::visit(
        [&key](const auto& val) -> bool {
            using clean_t = std::remove_cvref_t<decltype(val)>;

            if constexpr (std::same_as<clean_t, object_ptr_t>)
                return val->contains(std::string {key});
            return false;
        },
        _value);
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

auto json::clear(void) noexcept -> void {
    _hasValue = false;
    _value    = {};
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
