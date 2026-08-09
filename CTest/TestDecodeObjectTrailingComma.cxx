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
        (void)libjson::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
