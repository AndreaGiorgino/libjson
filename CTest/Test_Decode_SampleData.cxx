#include <fstream>

#include "helpers.hxx"

using libjson::json;
using libjson::object_t;

auto Test_Decode_SampleData(int, char**) -> int {
    const object_t expected {
        {"lastName",  "ABBING"    },
        {"firstName", "Mr Anthony"},
        {"age",       "41"        },
        {"paxClass",  "3rd Class" },
        {"passenger", true        },
        {"survivor",  false       }
    };

    std::ifstream ifs {"TestFiles/titanic-passengers.json"};

    const auto node {json::decode(ifs)};

    helpers::checkEq<std::string>(
        node.at(0).at("lastName"), expected.at("lastName"));
    helpers::checkEq<std::string>(
        node.at(0).at("firstName"), expected.at("firstName"));
    helpers::checkEq<std::string>(node.at(0).at("age"), expected.at("age"));
    helpers::checkEq<std::string>(
        node.at(0).at("paxClass"), expected.at("paxClass"));
    helpers::checkEq<bool>(
        node.at(0).at("passenger"), expected.at("passenger"));
    helpers::checkEq<bool>(node.at(0).at("survivor"), expected.at("survivor"));

    return 0;
}
