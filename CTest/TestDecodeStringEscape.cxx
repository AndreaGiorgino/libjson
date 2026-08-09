#include <sstream>

#include "helpers.hxx"
#include "libjson/extra/decode.hxx"

using std::string_literals::operator ""s;

auto TestDecodeStringEscape(int, char**) -> int {
    std::stringstream ss {R"(
    {
        "quote": "\"",
        "backslash": "\\",
        "forwardSlash": "\/",
        "backspace": "\b",
        "formFeed": "\f",
        "newline": "\n",
        "carriageReturn": "\r",
        "tab": "\t",
        "unicode": "\u03A0"
    }
    )"};

    const auto root {libjson::decode(ss)};
    helpers::check_eq(root.at("quote"), "\\\""s);
    helpers::check_eq(root.at("backslash"), "\\\\"s);
    helpers::check_eq(root.at("forwardSlash"), "\\/"s);
    helpers::check_eq(root.at("backspace"), "\\b"s);
    helpers::check_eq(root.at("formFeed"), "\\f"s);
    helpers::check_eq(root.at("newline"), "\\n"s);
    helpers::check_eq(root.at("carriageReturn"), "\\r"s);
    helpers::check_eq(root.at("tab"), "\\t"s);
    helpers::check_eq(root.at("unicode"), "\\u03A0"s);

    return 0;
}
