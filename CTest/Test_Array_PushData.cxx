#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::array_t;
using libjson::json;

auto Test_Array_PushData(int, char**) -> int {
    const std::string expected {"json"};

    // array initialised
    json el0 {array_t {}};
    el0.push_back(expected);
    helpers::checkEq(el0[0], expected);

    // null initialised
    json el1 {};
    el1.push_back(expected);
    helpers::checkEq(el1[0], expected);

    // move
    auto buffer {expected};

    json el2 {};
    el2.push_back(std::move(buffer));
    helpers::checkEq(el2[0], expected);

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    // trasform and push
    json el3 {69};
    el3.push_back(expected);
    helpers::checkEq(el3.at(0), 69);
    helpers::checkEq(el3.at(1), expected);

    return 0;
}
