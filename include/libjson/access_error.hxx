#pragma once

#include <string>

namespace libjson {
class access_error final : public std::exception {
   public: // ctors
    access_error(std::string_view message = "Unknown access error");

    access_error(const access_error&)                     = default;
    auto operator =(const access_error&) -> access_error& = default;

    access_error(access_error&&)                     = default;
    auto operator =(access_error&&) -> access_error& = default;

    ~access_error(void) = default;

   public: // methods
    auto what(void) const noexcept -> const char* override;

   private: // members
    std::string _message {};
};
} // namespace libjson
