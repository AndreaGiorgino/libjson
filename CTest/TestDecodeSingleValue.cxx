#include <sstream>

#include "helpers.hxx"
#include "libjson/extra/decode.hxx"

auto TestDecodeSingleValue(int, char**) -> int {
    const std::string expected {"json"};

    std::stringstream ss {R"("json")"};

    const auto el {libjson::decode(ss)};
    helpers::check_eq(el, expected);

    return 0;
}
