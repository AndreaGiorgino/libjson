#include <fstream>

#include "libjson/json.hxx"

using libjson::json;
using libjson::object_t;

auto TestEncode(int, char**) -> int {
    const object_t val {
        {"lastName",  "ABBING"    },
        {"firstName", "Mr Anthony"},
        {"age",       "41"        },
        {"paxClass",  "3rd Class" },
        {"passenger", true        },
        {"survivor",  false       }
    };

    const json node {val};
    const auto encoded {node.encode()};

    std::ifstream ifs {"TestFiles/sample-data.json"};
    std::string content {std::istreambuf_iterator {ifs}, {}};

    if (encoded != content) throw std::runtime_error("Values not equals");

    return 0;
}
