#include <cassert>

#include "libjson/json.hxx"

using json = libjson::json;

auto TestCtorsDouble(int, char**) -> int {
    // copy
    [[maybe_unused]] const double expected {420.0};
    assert(json {expected}.as<int>() == expected);
    assert((json {} = expected).as<int>() == expected);

    // move
    assert(json {420.0}.as<int>() == expected);
    assert((json {} = 420.0).as<int>() == expected);

    return 0;
}
