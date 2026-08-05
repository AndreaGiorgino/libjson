#include "libjson/extra/json.hxx"

#include <cassert>
#include <sstream>

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
 */
auto throw_eof(std::istream& is) -> void;

/**
 * @brief Skip whitespaces in stream
 *
 * @param is The input stream
 */
auto skipws(std::istream& is) noexcept -> void;

/**
 * @brief Parse from stream a json value
 *
 * @param is The input stream
 */
[[nodiscard]] auto parse(std::istream& is) -> json;

/**
 * @brief Parse from stream a json object value
 *
 * @param is The input stream
 */
[[nodiscard]] auto parse_object(std::istream& is) -> json;

/**
 * @brief Parse from stream a json array value
 *
 * @param is The input stream
 */
[[nodiscard]] auto parse_array(std::istream& is) -> json;

/**
 * @brief Parse from stream a json primitive value (bool/int/double/string)
 *
 * @param is The input stream
 */
[[nodiscard]] auto parse_value(std::istream& is) -> json;

// ----------------------------------------------------------------------------

auto throw_eof(std::istream& is) -> void {
    if (is.eof()) throw parse_error("Premature EOF encountered");
}

auto skipws(std::istream& is) noexcept -> void {
    while (!is.eof() && std::isspace(is.peek())) is.ignore();
}

auto parse_object(std::istream& is) -> json {
    if (is.eof()) throw parse_error("Premature EOF encountered");

    assert(is.peek() == '{');

    const auto start {is.tellg()};
    is.ignore();

    // TODO: parse keys
    // TODO: parse values

    if (is.eof() || is.peek() != '}')
        throw parse_error(std::format(
            "Unclosed object opened at position {}", (std::size_t)start + 1));

    is.ignore();
    return {};
}

auto parse_array(std::istream& is) -> json {
    if (is.eof()) throw parse_error("Premature EOF encountered");

    assert(is.peek() == '[');

    const auto start {is.tellg()};
    is.ignore();

    // TODO: parse values

    if (is.eof() || is.peek() != ']')
        throw parse_error(std::format(
            "Unclosed object opened at position {}", (std::size_t)start + 1));

    is.ignore();
    return {};
}

auto parse_value(std::istream& is) -> json {
    if (is.eof()) throw parse_error("Premature EOF encountered");

    const auto ch {is.peek()};
    if (ch == '"') {
        // TODO: decode string
    } else if (std::isdigit(ch)) {
        // TODO: parse number
    } else if (std::isalpha(ch)) {
        // TODO: check for keywords
    }

    throw parse_error(std::format("Unexpected character at position {}: {}",
        (std::size_t)is.tellg() + 1, ch));

    return {};
}

auto parse(std::istream& is) -> json {
    skipws(is);

    if (is.eof())
        throw parse_error("Premature EOF encountered");
    else if (is.peek() == '{')
        return parse_object(is);
    else if (is.peek() == '[')
        return parse_array(is);
    return parse_value(is);
}

auto decode(std::string_view raw) -> json {
    return decode(std::stringstream {raw.data()});
}

auto decode(std::istream&& is) -> json {
    if (!is) throw std::runtime_error("Invalid stream provided");

    const auto ret {parse(is)};

    skipws(is);
    if (!is.eof()) throw parse_error("Single value expected at root level");

    return ret;
}
} // namespace libjson
