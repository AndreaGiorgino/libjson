#include <libjson/json.hxx>
#include <libjson/parse_error.hxx>
#include <sstream>

using libjson::json;

auto TestDecodeObjectUnclosed(int, char**) -> int {
    std::stringstream ss {"{"};

    try {
        (void)json::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
