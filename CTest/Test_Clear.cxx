#include <libjson/json.hxx>

using libjson::json;

auto Test_Clear(int, char**) -> int {
    json val {1};

    val.clear();

    if (val.has_value())
        std::runtime_error("Node was not cleared");

    return 0;
}
