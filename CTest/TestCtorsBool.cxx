#include <cassert>

#include "libjson/json.hxx"

using json = libjson::json;

auto TestCtorsBool(int, char**) -> int {
    // copy
    [[maybe_unused]] const bool expected {true};
    assert(json {expected}.as<bool>() == expected);
    assert((json {} = expected).as<bool>() == expected);

    // move
    assert(json {true}.as<bool>() == expected);
    assert((json {} = true).as<bool>() == expected);

    return 0;
}
