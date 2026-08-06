#include "helpers.hxx"
#include "libjson/json.hxx"

using json     = libjson::json;
using object_t = libjson::object_t;

auto TestCtorsObject(int, char**) -> int {
    const object_t expected {
        {"number",  69    },
        {"string", "json"},
    };

    // copy
    const json el0 {expected};
    helpers::check_child_eq(el0, "number", expected.at("number").as<int>());
    helpers::check_child_eq(el0, "string", expected.at("string").as<std::string>());

    const auto el1(json {} = expected);
    helpers::check_child_eq(el1, "number", expected.at("number").as<int>());
    helpers::check_child_eq(el1, "string", expected.at("string").as<std::string>());

    // move
    auto buffer = expected;

    const json el2 {std::move(buffer)};
    helpers::check_child_eq(el2, "number", expected.at("number").as<int>());
    helpers::check_child_eq(el2, "string", expected.at("string").as<std::string>());

    if (!buffer.empty()) throw "Buffer not moved";

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::check_child_eq(el3, "number", expected.at("number").as<int>());
    helpers::check_child_eq(el3, "string", expected.at("string").as<std::string>());

    if (!buffer.empty()) throw "Buffer not moved";

    return 0;
}
