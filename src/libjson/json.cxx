#include "libjson/json.hxx"

#include <cassert>
#include <sstream>

#include "libjson/parse_error.hxx"

namespace libjson {
/**
 * @brief Skip whitespaces in stream
 *
 * @param is The input stream
 */
auto _skipws(std::istream& is) noexcept -> void;

/**
 * @brief Parse from stream a json value
 *
 * @param is The input stream
 */
auto _parse(std::istream& is) -> json;

/**
 * @brief Parse from stream a json object value
 *
 * @param is The input stream
 */
auto _parse_object(std::istream& is) -> json;

/**
 * @brief Parse from stream a json array value
 *
 * @param is The input stream
 */
auto _parse_array(std::istream& is) -> json;

/**
 * @brief Parse from stream a json fundamental value (bool/int/double/string)
 *
 * @param is The input stream
 */
auto _parse_value(std::istream& is) -> json;

// ----------------------------------------------------------------------------

auto json::has_value(void) const noexcept -> bool {
    return _hasValue;
}

auto _skipws(std::istream& is) noexcept -> void {
    while (!is.eof() && std::isspace(is.peek())) is.ignore();
}

auto _parse_object(std::istream& is) -> json {
    assert(is.peek() == '{');

    const auto start {is.tellg()};
    is.ignore();
    _skipws(is);

    const auto key {_parse(is)};

    return {};
}

auto _parse_array(std::istream& is) -> json {
    assert(is.peek() == '[');
    return {};
}

auto _parse_value(std::istream& is) -> json {
    return {};
}

auto _parse(std::istream& is) -> json {
    _skipws(is);

    if (is.eof())
        throw parse_error("Stream reached premature EOF");
    else if (is.peek() == '{')
        return _parse_object(is);
    else if (is.peek() == '[')
        return _parse_array(is);
    return _parse_value(is);
}

auto load(std::string_view raw) -> json {
    return load(std::stringstream {raw.data()});
}

auto load(std::istream&& is) -> json {
    if (!is) throw std::runtime_error("Invalid stream provided");

    const auto ret {_parse(is)};

    _skipws(is);
    if (!is.eof())
        throw parse_error("Only a single value expected at root level");

    return ret;
}
} // namespace libjson
