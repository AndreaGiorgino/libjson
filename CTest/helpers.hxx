#pragma once

#include <string_view>

#include "libjson/json.hxx"

namespace helpers {
template <typename T>
    requires libjson::variant_member_v<T>
auto check_eq(libjson::json lhs, T rhs) -> void {
    if (lhs.as<T>() != rhs) throw "Values not equals";
}

template <typename Key, typename T>
    requires(
        std::integral<Key> || std::same_as<Key, std::string_view>)
auto check_child_eq(libjson::json lhs, Key key, T rhs) -> void {
    if (lhs.at(key).template as<T>() != rhs) throw "Values not equals";
}
} // namespace helpers
