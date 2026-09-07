#include <libjson/json.hxx>
#include <libjson/parse_error.hxx>
#include <sstream>

using libjson::json;

auto Test_Decode_ObjectTrailingComma(int, char**) -> int {
    std::stringstream ss {R"(
        {
            "test": null,
        }
    )"};

    try {
        (void)json::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
