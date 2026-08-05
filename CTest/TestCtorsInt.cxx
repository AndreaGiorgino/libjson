#include <cassert>

#include "libjson/json.hxx"

using json = libjson::json;

auto TestCtorsInt(int, char**) -> int {
    // copy
    [[maybe_unused]] const int expected {69};
    assert(json {expected}.as<int>() == expected);
    assert((json {} = expected).as<int>() == expected);

    // move
    assert(json {69}.as<int>() == expected);
    assert((json {} = 69).as<int>() == expected);

    return 0;
}
