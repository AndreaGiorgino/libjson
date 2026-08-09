#include <sstream>

#include "libjson/extra/decode.hxx"
#include "libjson/parse_error.hxx"

auto TestDecodeStringInvalidEscape(int, char**) -> int {
    // parse_error
    /// invalid escape sequence
    try {
        auto ss {std::stringstream {R"("\v")"}};
        (void)libjson::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    /// invalid unicode
    try {
        auto ss {std::stringstream {R"("\uXXXX")"}};
        (void)libjson::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
