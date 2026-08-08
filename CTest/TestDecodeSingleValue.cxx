#include <fstream>

#include "helpers.hxx"
#include "libjson/extra/decode.hxx"

auto TestDecodeSingleValue(int, char**) -> int {
    const std::string expected {"json"};

    std::ifstream ifs {"TestFiles/single-value.json"};

    const auto el {libjson::decode(ifs)};
    helpers::check_eq(el, expected);

    return 0;
}
