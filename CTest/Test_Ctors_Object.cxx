#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::json;
using libjson::object_t;

auto Test_Ctors_Object(int, char**) -> int {
    const object_t expected {
        {"number", 69    },
        {"string", "json"},
    };

    // copy
    const json el0 {expected};
    helpers::checkEq<int>(el0.at("number"), expected.at("number"));
    helpers::checkEq<std::string>(el0.at("string"), expected.at("string"));

    const auto el1(json {} = expected);
    helpers::checkEq<int>(el1.at("number"), expected.at("number"));
    helpers::checkEq<std::string>(el1.at("string"), expected.at("string"));

    // move
    auto buffer = expected;

    const json el2 {std::move(buffer)};
    helpers::checkEq<int>(el2.at("number"), expected.at("number"));
    helpers::checkEq<std::string>(el2.at("string"), expected.at("string"));

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::checkEq<int>(el3.at("number"), expected.at("number"));
    helpers::checkEq<std::string>(el3.at("string"), expected.at("string"));

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    return 0;
}
