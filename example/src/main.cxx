#include <iomanip>
#include <iostream>
#include <print>

#include "libjson/extra/decode.hxx"

using namespace libjson;

auto main(int, char**) -> int {
    const char* raw {R"(
    {
        "name": "John Smith",
        "age": 25,
        "city": {
            "name": "London"
        }
    }
    )"};

    auto node {libjson::decode(raw)};

    // encode to string
    std::println("{:-^30}", "[String encode]");
    std::println("{}\n", node.encode());

    // encode to osteam with indentation
    std::println("{:-^30}", "[Ostream insert]");
    std::cout << std::setw(4) << node << "\n\n";

    // example usage
    std::println("{:-^30}", "[Example usage]");
    std::cout << node["name"] << " from " << node["city"]["name"] << ", is "
              << node["age"] << " years old." << std::endl;

    return 0;
}
