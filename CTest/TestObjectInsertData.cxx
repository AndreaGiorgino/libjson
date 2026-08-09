#include "helpers.hxx"
#include "libjson/json.hxx"

using libjson::json;
using libjson::object_t;

auto TestObjectInsertData(int, char**) -> int {
    const std::string expected {"json"};

    // array initialised
    json el0 {object_t {}};
    el0.insert("key", expected);
    helpers::check_eq(el0.at("key"), expected);

    // null initialised
    json el1 {};
    el1.insert("key", expected);
    helpers::check_eq(el1.at("key"), expected);

    // move
    auto buffer {expected};

    json el2 {};
    el2.insert("key", std::move(buffer));
    helpers::check_eq(el2.at("key"), expected);

    if (!buffer.empty()) throw std::runtime_error("Buffer not moved");

    // bad_variant_access
    try {
        json {69}.insert("", {});
        throw;
    } catch (const std::bad_variant_access&) {
    } catch (...) {
        throw;
    }

    return 0;
}
