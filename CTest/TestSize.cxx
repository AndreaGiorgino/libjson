#include "libjson/json.hxx"

using libjson::array_t;
using libjson::json;
using libjson::object_t;

auto TestSize(int, char**) -> int {
    if (json {}.size() != 0)
        throw std::runtime_error("Values not equals");
    else if (json {0}.size() != 1)
        throw std::runtime_error("Values not equals");
    else if (json {array_t {}}.size() != 0)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {}}.size() != 0)
        throw std::runtime_error("Values not equals");
    else if (json {array_t {0}}.size() != 1)
        throw std::runtime_error("Values not equals");
    else if (json {object_t {{"key", 0}}}.size() != 1)
        throw std::runtime_error("Values not equals");

    return 0;
}
