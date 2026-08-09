#include <sstream>

#include "libjson/parse_error.hxx"
#include "libjson/extra/decode.hxx"

auto TestDecodeObjectUnclosed(int, char**) -> int {
    std::stringstream ss {"{"};

    try {
        (void)libjson::decode(ss);
        throw;
    } catch (const libjson::parse_error&) {
    } catch (...) {
        throw;
    }

    return 0;
}
