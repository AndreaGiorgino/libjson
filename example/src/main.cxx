#include <iostream>
#include <sstream>

#include "libjson/extra/decode.hxx"

using namespace libjson;

auto main(int, char**) -> int {
    std::stringstream ss {R"(
    {
        "name": "John Smith",
        "age": 25,
        "city": {
            "name": "London"
        }
    }
    )"};

    auto root {libjson::decode(ss)};
    std::cout << root["name"].as<std::string>() << " from "
              << root["city"]["name"].as<std::string>() << " is "
              << root["age"].as<int>() << " years old." << std::endl;

    return 0;
}
