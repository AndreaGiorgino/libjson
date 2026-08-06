#include "helpers.hxx"
#include "libjson/json.hxx"

using json = libjson::json;

auto TestCtorsDouble(int, char**) -> int {
    const double expected {69.420};

    // copy
    const json el0 {expected};
    helpers::check_eq(el0, expected);

    const auto el1(json {} = expected);
    helpers::check_eq(el1, expected);

    // move
    const auto el2 {69.420};
    helpers::check_eq(el2, expected);

    const auto el3(json {} = 69.420);
    helpers::check_eq(el1, expected);

    return 0;
}
