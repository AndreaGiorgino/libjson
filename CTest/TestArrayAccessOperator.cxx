#include "libjson/json.hxx"

using libjson::json;
using libjson::array_t;

auto TestArrayAccessOperator(int, char**) -> int {
    // bad_variant_access
    try {
        (void)json {69}[0];
    } catch (const std::bad_variant_access&) {
    } catch (...) {
        throw;
    }

    // out_of_range
    try {
        (void)json {array_t {}}[0];
    } catch (const std::out_of_range&) {
    } catch (...) {
        throw;
    }

    return 0;
}
