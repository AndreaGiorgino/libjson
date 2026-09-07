#include <libjson/parse_error.hxx>

namespace libjson {
parse_error::parse_error(std::string_view message) : _message(message.data()) {}

auto parse_error::what(void) const noexcept -> const char* {
    return _message.data();
}
} // namespace libjson
