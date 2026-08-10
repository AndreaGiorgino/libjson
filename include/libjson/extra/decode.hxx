#pragma once

#include "libjson/json.hxx"

namespace libjson {
/**
 * @brief Decode json from raw string
 *
 * @param raw The input raw json
 * @return The encoded std::string_view as json
 *
 * @throws parse_error If the stream is malformed
 */
[[nodiscard]] auto decode(std::string_view raw) -> json;

/**
 * @brief Decode json from stream
 *
 * @param is The input stream
 * @return The encoded std::string_view as json
 *
 * @throws parse_error If the stream is malformed
 */
[[nodiscard]] auto decode(std::istream& is) -> json;
} // namespace libjson
