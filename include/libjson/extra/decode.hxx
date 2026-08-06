#pragma once

#include "libjson/json.hxx"

namespace libjson {
/**
 * @brief Encode json into string
 *
 * @param el The json element
 * @return The encoded json as std::string
 *
 * @throws parse_error If the stream is malformed
 */
auto encode(json&& el) -> std::string;

/**
 * @brief Encode json into stream
 *
 * @param el The json element
 * @param os The output stream
 * @return The encoded json as std::string
 *
 * @throws parse_error If the stream is malformed
 */
auto encode(json&& el, std::ostream& os) -> void;

/**
 * @brief Decode json from raw string
 *
 * @param raw The input raw json
 * @return The encoded std::string_view as json
 *
 * @throws parse_error If the stream is malformed
 */
auto decode(std::string_view raw) -> json;

/**
 * @brief Decode json from stream
 *
 * @param is The input stream
 * @return The encoded std::string_view as json
 *
 * @throws parse_error If the stream is malformed
 */
auto decode(std::istream& is) -> json;
} // namespace libjson
