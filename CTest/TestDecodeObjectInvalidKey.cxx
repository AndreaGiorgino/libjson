#include <libjson/json.hxx>
#include <libjson/parse_error.hxx>
#include <sstream>

using libjson::json;

auto TestDecodeObjectInvalidKey(int, char**) -> int {
    std::stringstream ss {R"(
        {
            0: null,
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
