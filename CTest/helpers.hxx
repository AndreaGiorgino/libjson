#pragma once

#include <libjson/json.hxx>

namespace helpers {
template <typename Tp>
    requires libjson::variant_member_v<Tp>
auto check_eq(libjson::json lhs, Tp rhs) -> void {
    if (lhs.as<Tp>() != rhs) throw std::runtime_error("Values not equals");
}

template <typename Tp>
    requires libjson::variant_member_v<Tp>
auto check_eq(libjson::json lhs, libjson::json rhs) -> void {
    if (lhs.as<Tp>() != rhs.as<Tp>())
        throw std::runtime_error("Values not equals");
}
} // namespace helpers
