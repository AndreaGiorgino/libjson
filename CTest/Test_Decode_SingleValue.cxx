#include <libjson/json.hxx>
#include <sstream>

#include "helpers.hxx"

using libjson::json;

auto Test_Decode_SingleValue(int, char**) -> int {
    const std::string expected {"json"};

    std::stringstream ss {R"("json")"};

    const auto el {json::decode(ss)};
    helpers::checkEq(el, expected);

    return 0;
}
