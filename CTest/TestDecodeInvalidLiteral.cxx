#include <sstream>

#include "libjson/parse_error.hxx"
#include "libjson/extra/decode.hxx"

auto TestDecodeInvalidLiteral(int, char**) -> int {
    std::stringstream ss {"test"};

    try {
        (void)libjson::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
