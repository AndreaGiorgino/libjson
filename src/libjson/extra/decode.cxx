#include <cassert>
#include <functional>
#include <sstream>

#include "libjson/json.hxx"
#include "libjson/parse_error.hxx"

namespace libjson {
auto encode(json&& el) -> std::string {
    // TODO: not implemented yet
    (void)el;
    throw;
}

auto encode(json&& el, std::ostream& os) -> void {
    // TODO: not implemented yet
    (void)el;
    (void)os;
    throw;
}

// ----------------------------------------------------------------------------

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

auto decode(std::istream& is) -> json {
    if (!is) throw std::runtime_error("Invalid stream provided");

    skipws(is);
    if (is.eof()) return {};

    const auto ret {parse(is)};

    skipws(is);
    if (!is.eof()) throw parse_error("Single value expected at root level");

    return ret;
}

auto decode(std::string_view raw) -> json {
    std::istringstream ss {raw.data()};
    return decode(ss);
}
} // namespace libjson
