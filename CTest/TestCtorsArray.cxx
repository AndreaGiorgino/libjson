#include "helpers.hxx"
#include "libjson/json.hxx"

using json    = libjson::json;
using array_t = libjson::array_t;

auto TestCtorsArray(int, char**) -> int {
    const array_t expected {69, "json"};

    // copy
    const json el0 {expected};
    helpers::check_child_eq(el0, 0, expected[0].as<int>());
    helpers::check_child_eq(el0, 1, expected[1].as<std::string>());

    const auto el1(json {} = expected);
    helpers::check_child_eq(el1, 0, expected[0].as<int>());
    helpers::check_child_eq(el1, 1, expected[1].as<std::string>());

    // move
    auto buffer = expected;

    const json el2 {std::move(buffer)};
    helpers::check_child_eq(el2, 0, expected[0].as<int>());
    helpers::check_child_eq(el2, 1, expected[1].as<std::string>());

    if (!buffer.empty()) throw "Buffer not moved";

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::check_child_eq(el3, 0, expected[0].as<int>());
    helpers::check_child_eq(el3, 1, expected[1].as<std::string>());

    if (!buffer.empty()) throw "Buffer not moved";

    return 0;
}
