#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::json;

auto Test_Ctors_Double(int, char**) -> int {
    const double expected {69.420};

    // copy
    const json el0 {expected};
    helpers::checkEq(el0, expected);

    const auto el1(json {} = expected);
    helpers::checkEq(el1, expected);

    // move
    const auto el2 {69.420};
    helpers::checkEq(el2, expected);

    const auto el3(json {} = 69.420);
    helpers::checkEq(el1, expected);

    return 0;
}
