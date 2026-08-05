#include "libjson/access_error.hxx"

namespace libjson {
access_error::access_error(std::string_view message) : _message(message.data()) {}

auto access_error::what(void) const noexcept -> const char* {
    return _message.data();
}
} // namespace libjson
