#include <iomanip>
#include <iostream>
#include <libjson/json.hxx>
#include <print>

using libjson::json;

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

    auto node {json::decode(raw)};

    // encode to string
    std::println("{:-^30}", "[String encode]");
    std::println("{}\n", node.encode());

    // encode to osteam with indentation
    std::println("{:-^30}", "[Ostream insert]");
    std::cout << std::setw(4) << node << "\n\n";

    // chain insert
    node["city"]["coordinates"]["lat"] = 51.52817792811963;
    node["city"]["coordinates"]["lon"] = -0.13045990894445286;

    std::println("{:-^30}", "[Ostream insert (updated)]");
    std::cout << std::setw(4) << node << "\n\n";

    return 0;
}
