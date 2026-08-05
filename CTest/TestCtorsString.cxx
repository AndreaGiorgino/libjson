#include <cassert>

#include "libjson/json.hxx"

using json = libjson::json;

auto TestCtorsString(int, char**) -> int {
    // copy
    const std::string expected {"hello world"};
    assert(json {expected}.as<std::string>() == expected);
    assert((json {} = expected).as<std::string>() == expected);

    // move
    assert(json {"hello world"}.as<std::string>() == expected);
    assert((json {} = "hello world").as<std::string>() == expected);

    auto buffer {expected};
    assert(json {std::move(buffer)}.as<std::string>() == expected);
    assert(buffer.empty());

    return 0;
}
