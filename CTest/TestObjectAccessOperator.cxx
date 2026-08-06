#include "helpers.hxx"
#include "libjson/json.hxx"

using json     = libjson::json;
using object_t = libjson::object_t;

auto TestObjectAccessOperator(int, char**) -> int {
    const int expected {69};

    json el0 {};
    el0["number"] = 69;
    helpers::check_child_eq(el0, "number", expected);

    try {
        json el1 {69};
        (void)el1[""];
    } catch (const std::bad_variant_access& ex) {
    } catch (...) {
        throw;
    }

    return 0;
}
