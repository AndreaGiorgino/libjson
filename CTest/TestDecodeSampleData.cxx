#include <fstream>

#include "helpers.hxx"
#include "libjson/extra/decode.hxx"

using libjson::object_t;

auto TestDecodeSampleData(int, char**) -> int {
    const object_t expected {
        {"lastName",  "ABBING"    },
        {"firstName", "Mr Anthony"},
        {"age",       "41"        },
        {"paxClass",  "3rd Class" },
        {"passenger", true        },
        {"survivor",  false       }
    };

    std::ifstream ifs {"TestFiles/titanic-passengers.json"};

    const auto root {libjson::decode(ifs)};

    helpers::check_eq<std::string>(
        root.at(0).at("lastName"), expected.at("lastName"));
    helpers::check_eq<std::string>(
        root.at(0).at("firstName"), expected.at("firstName"));
    helpers::check_eq<std::string>(root.at(0).at("age"), expected.at("age"));
    helpers::check_eq<std::string>(
        root.at(0).at("paxClass"), expected.at("paxClass"));
    helpers::check_eq<bool>(
        root.at(0).at("passenger"), expected.at("passenger"));
    helpers::check_eq<bool>(root.at(0).at("survivor"), expected.at("survivor"));

    return 0;
}
