#include "helpers.hxx"
#include "libjson/json.hxx"

using json    = libjson::json;
using array_t = libjson::array_t;

auto TestArrayPushData(int, char**) -> int {
    const std::string expected {"json"};

    // array initialised
    json el0 {array_t {}};
    el0.push_back(expected);
    helpers::check_eq(el0[0], expected);

    // null initialised
    json el1 {};
    el1.push_back(expected);
    helpers::check_eq(el1[0], expected);

    // move
    auto buffer {expected};

    json el2 {};
    el2.push_back(std::move(buffer));
    helpers::check_eq(el2[0], expected);

    if (!buffer.empty()) throw "buffer not moved";

    // trasform and push
    json el3 {69};
    el3.push_back(expected);
    helpers::check_eq(el3[0], 69);
    helpers::check_eq(el3[1], expected);

    return 0;
}
