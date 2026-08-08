#include "helpers.hxx"
#include "libjson/json.hxx"

using json    = libjson::json;
using array_t = libjson::array_t;

auto TestCtorsArray(int, char**) -> int {
    const array_t expected {69, "json"};

    // copy
    const json el0 {expected};
    helpers::check_eq<int>(el0.at(0), expected[0]);
    helpers::check_eq<std::string>(el0.at(1), expected[1]);

    const auto el1(json {} = expected);
    helpers::check_eq<int>(el1.at(0), expected[0]);
    helpers::check_eq<std::string>(el1.at(1), expected[1]);

    // move
    auto buffer = expected;

    const json el2 {std::move(buffer)};
    helpers::check_eq<int>(el2.at(0), expected[0]);
    helpers::check_eq<std::string>(el2.at(1), expected[1]);

    if (!buffer.empty()) throw "Buffer not moved";

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::check_eq<int>(el3.at(0), expected[0]);
    helpers::check_eq<std::string>(el3.at(1), expected[1]);

    if (!buffer.empty()) throw "Buffer not moved";

    return 0;
}
