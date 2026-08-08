#include <fstream>

#include "libjson/extra/decode.hxx"

auto TestDecodeEmpty(int, char**) -> int {
    std::ifstream ifs {"TestFiles/empty.json"};

    const auto el {libjson::decode(ifs)};
    if (el.has_value()) throw "Unexpected content";

    return 0;
}
