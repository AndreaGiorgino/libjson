#include <libjson/json.hxx>
#include <sstream>

#include "helpers.hxx"

using libjson::json;

auto TestDecodeSingleValue(int, char**) -> int {
    const std::string expected {"json"};

    std::stringstream ss {R"("json")"};

    const auto el {json::decode(ss)};
    helpers::check_eq(el, expected);

    return 0;
}
