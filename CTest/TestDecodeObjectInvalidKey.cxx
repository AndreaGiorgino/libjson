#include <sstream>

#include "libjson/parse_error.hxx"
#include "libjson/extra/decode.hxx"

auto TestDecodeObjectInvalidKey(int, char**) -> int {
    std::stringstream ss {R"(
        {
            0: null,
        }
    )"};

    try {
        libjson::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
