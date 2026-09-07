#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::json;

auto TestCtorsInt(int, char**) -> int {
    const int expected {69};

    // copy
    const json el0 {expected};
    helpers::check_eq(el0, expected);

    const auto el1(json {} = expected);
    helpers::check_eq(el1, expected);

    // move
    const auto el2 {69};
    helpers::check_eq(el2, expected);

    const auto el3(json {} = 69);
    helpers::check_eq(el1, expected);

    return 0;
}
