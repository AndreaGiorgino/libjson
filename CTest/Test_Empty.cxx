#include <libjson/json.hxx>

using libjson::array_t;
using libjson::json;
using libjson::object_t;

auto Test_Empty(int, char**) -> int {
    if (json {}.empty() != true)
        throw std::runtime_error("Values not equals");
    else if (json {0}.empty() != false)
        throw std::runtime_error("Values not equals");
    else if (json {array_t {}}.empty() != true)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {}}.empty() != true)
        throw std::runtime_error("Values not equals");
    else if (json {array_t {0}}.empty() != false)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {{"key", 0}}}.empty() != false)
        throw std::runtime_error("Values not equals");

    return 0;
}
