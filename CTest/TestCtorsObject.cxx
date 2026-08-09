#include "helpers.hxx"
#include "libjson/json.hxx"

using libjson::json;
using libjson::object_t;

auto TestCtorsObject(int, char**) -> int {
    const object_t expected {
        {"number", 69    },
        {"string", "json"},
    };

    // copy
    const json el0 {expected};
    helpers::check_eq<int>(el0.at("number"), expected.at("number"));
    helpers::check_eq<std::string>(el0.at("string"), expected.at("string"));

    const auto el1(json {} = expected);
    helpers::check_eq<int>(el1.at("number"), expected.at("number"));
    helpers::check_eq<std::string>(el1.at("string"), expected.at("string"));

    // move
    auto buffer = expected;

    const json el2 {std::move(buffer)};
    helpers::check_eq<int>(el2.at("number"), expected.at("number"));
    helpers::check_eq<std::string>(el2.at("string"), expected.at("string"));

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::check_eq<int>(el3.at("number"), expected.at("number"));
    helpers::check_eq<std::string>(el3.at("string"), expected.at("string"));

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    return 0;
}
