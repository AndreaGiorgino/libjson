#include "libjson/extra/json.hxx"

#include <cassert>
#include <functional>
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
 * @brief Get characters until the stop condition is met
 *
 * @param is The input stream
 * @param stop_condition
 */
[[nodiscard]] auto get_until(std::istream& is,
    std::function<char(bool)> stop_condition) noexcept -> std::string;

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

auto get_until(std::istream& is,
    std::function<char(bool)> stop_condition) noexcept -> std::string {
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

    // TODO: parse keys
    // TODO: parse values

    if (is.eof() || is.peek() != '}')
        throw parse_error(std::format(
            "Unclosed object opened at position {}", (std::size_t)start + 1));

    is.ignore();
    return {};
}

auto parse_array(std::istream& is) -> json {
    throw_eof(is);
    assert(is.peek() == '[');

    const auto start {is.tellg()};
    is.ignore();

    array_t buffer {};
    while (!is.eof()) {
        if (is.peek() == ']') {
            if (buffer.size() == 0)
                break;
            else
                throw parse_error(
                    std::format("Trailing comma encountered at position {}",
                        (std::size_t)start + 1));
        }

        buffer.push_back(parse(is));

        skipws(is);
        if (is.eof()) break;

        const auto ch {is.peek()};
        if (ch == ']')
            break;
        else if (ch == ',')
            is.ignore();
        else
            throw parse_error(
                std::format("Unexpected character at position {}: {}",
                    (std::size_t)start + 1, ch));
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
        std::string buffer {get_until(
            is, [](const auto ch) { return ch == '"' || ch == '\n'; })};

        if (is.eof() || is.peek() != '"')
            throw parse_error(
                std::format("Unclosed string opened at position {}: {}",
                    (std::size_t)start + 1, buffer));

        is.ignore();
        return buffer;
    } else if (std::isdigit(ch)) {
        std::string buffer {get_until(
            is, [](const auto ch) { return !std::isdigit(ch) && ch != '.'; })};

        try {
            if (buffer.find('.') != std::string::npos)
                return {std::stod(buffer)};
            else
                return {std::stoi(buffer)};
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
            return {false};
        else if (buffer == "true")
            return {true};

        throw parse_error(std::format("Unexpected literal at position {}: {}",
            (std::size_t)start + 1, buffer));
    }

    throw parse_error(std::format("Unexpected character at position {}: {}",
        (std::size_t)is.tellg() + 1, ch));
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
