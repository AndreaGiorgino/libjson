#pragma once

#include "libjson/json.hxx"

namespace libjson {
/**
 * @brief Decode json from raw string
 *
 * @param raw The input raw json
 */
auto decode(std::string_view raw) -> json;

/**
 * @brief Decode json from stream
 *
 * @param is The input stream
 */
auto decode(std::istream&& is) -> json;

/**
 * @brief Encode json into string
 *
 * @param el The json element
 */
auto encode(json&& el) -> std::string;

/**
 * @brief Encode json into stream
 *
 * @param el The json element
 * @param os The output stream
 */
auto encode(json&& el, std::ostream& os) -> void;
} // namespace libjson
