#include <libjson/json.hxx>

#include "helpers.hxx"

using libjson::array_t;
using libjson::json;

auto Test_Ctors_Array(int, char**) -> int {
    const array_t expected {69, "json"};

    // copy
    const json el0 {expected};
    helpers::checkEq<int>(el0.at(0), expected[0]);
    helpers::checkEq<std::string>(el0.at(1), expected[1]);

    const auto el1(json {} = expected);
    helpers::checkEq<int>(el1.at(0), expected[0]);
    helpers::checkEq<std::string>(el1.at(1), expected[1]);

    // move
    auto buffer = expected;

    const json el2 {std::move(buffer)};
    helpers::checkEq<int>(el2.at(0), expected[0]);
    helpers::checkEq<std::string>(el2.at(1), expected[1]);

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    buffer = expected;

    const auto el3(json {} = std::move(buffer));
    helpers::checkEq<int>(el3.at(0), expected[0]);
    helpers::checkEq<std::string>(el3.at(1), expected[1]);

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    return 0;
}
