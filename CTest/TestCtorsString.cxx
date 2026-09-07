#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::json;

auto TestCtorsString(int, char**) -> int {
    const std::string expected {"json"};

    // copy
    const json el0 {expected};
    helpers::check_eq(el0, expected);

    const auto el1(json {} = expected);
    helpers::check_eq(el1, expected);

    // move
    auto buffer {expected};

    const json el2 {std::move(buffer)};
    helpers::check_eq(el2, expected);

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::check_eq(el3, expected);

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    return 0;
}
