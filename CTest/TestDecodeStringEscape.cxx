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

    const auto node {libjson::decode(ss)};
    helpers::check_eq(node.at("quote"), "\""s);
    helpers::check_eq(node.at("backslash"), "\\"s);
    helpers::check_eq(node.at("forwardSlash"), "/"s);
    helpers::check_eq(node.at("backspace"), "\b"s);
    helpers::check_eq(node.at("formFeed"), "\f"s);
    helpers::check_eq(node.at("newline"), "\n"s);
    helpers::check_eq(node.at("carriageReturn"), "\r"s);
    helpers::check_eq(node.at("tab"), "\t"s);
    helpers::check_eq(node.at("unicode"), "\u03A0"s);

    return 0;
}
