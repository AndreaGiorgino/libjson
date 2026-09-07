#include <libjson/json.hxx>

using libjson::array_t;
using libjson::json;
using libjson::object_t;

auto Test_Object_Contains(int, char**) -> int {
    if (json {}.contains("") != false)
        throw std::runtime_error("Values not equals");
    else if (json {0}.contains("") != false)
        throw std::runtime_error("Values not equals");
    else if (json {array_t {}}.contains("") != false)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {}}.contains("") != false)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {{"key", 0}}}.contains("") != false)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {{"key", 0}}}.contains("key") != true)
        throw std::runtime_error("Values not equals");

    return 0;
}
