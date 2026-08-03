#include "libjson/json.hxx"

#include <cassert>

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
} // namespace libjson
