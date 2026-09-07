#include <libjson/json.hxx>
#include <libjson/parse_error.hxx>
#include <sstream>

using libjson::json;

auto Test_Decode_StringInvalidEscape(int, char**) -> int {
    // parse_error
    /// invalid escape sequence
    try {
        auto ss {std::stringstream {R"("\v")"}};
        (void)json::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    /// invalid unicode
    try {
        auto ss {std::stringstream {R"("\uXXXX")"}};
        (void)json::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
