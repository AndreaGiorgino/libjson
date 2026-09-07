#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::json;

auto Test_Ctors_Bool(int, char**) -> int {
    const bool expected {true};

    // copy
    const json el0 {expected};
    helpers::checkEq(el0, expected);

    const auto el1(json {} = expected);
    helpers::checkEq(el1, expected);

    // move
    const auto el2 {true};
    helpers::checkEq(el2, expected);

    const auto el3(json {} = true);
    helpers::checkEq(el1, expected);

    return 0;
}
