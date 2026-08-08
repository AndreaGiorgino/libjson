#include <sstream>

#include "libjson/parse_error.hxx"
#include "libjson/extra/decode.hxx"

auto TestDecodeObjectTrailingComma(int, char**) -> int {
    std::stringstream ss {R"(
        {
            "test": null,
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
