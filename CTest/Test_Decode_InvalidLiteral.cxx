#include <libjson/json.hxx>
#include <libjson/parse_error.hxx>
#include <sstream>

using libjson::json;

auto Test_Decode_InvalidLiteral(int, char**) -> int {
    std::stringstream ss {"test"};

    try {
        (void)json::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
