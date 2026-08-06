#include "helpers.hxx"
#include "libjson/json.hxx"

using json     = libjson::json;
using object_t = libjson::object_t;

auto TestObjectAccessOperator(int, char**) -> int {
    const int expected {69};

    // access existing key
    json el0 {object_t {{"number", 420}}};
    el0["number"] = expected;
    helpers::check_child_eq(el0, "number", expected);

    // access non-existing key
    /// object initialised
    json el1 {};
    el1["number"] = expected;
    helpers::check_child_eq(el1, "number", expected);

    /// null initialised
    json el2 {};
    el2["number"] = expected;
    helpers::check_child_eq(el2, "number", expected);

    // access nested key
    json el3 {};
    el3["nested"]["number"] = expected;
    helpers::check_child_eq(el3.at("nested"), "number", expected);

    // bad_variant_access
    try {
        (void)json {69}.at("");
    } catch (const std::bad_variant_access&) {
    } catch (...) {
        throw;
    }

    try {
        (void)json {69}[""];
    } catch (const std::bad_variant_access&) {
    } catch (...) {
        throw;
    }

    // out_of_range
    try {
        (void)json {object_t {}}.at("");
    } catch (const std::out_of_range&) {
    } catch (...) {
        throw;
    }

    return 0;
}
