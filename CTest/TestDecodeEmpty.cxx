#include <sstream>
#include <libjson/json.hxx>

using libjson::json;

auto TestDecodeEmpty(int, char**) -> int {
    std::stringstream ss {};

    const auto el {json::decode(ss)};
    if (el.has_value()) throw std::runtime_error("Unexpected content");

    return 0;
}
