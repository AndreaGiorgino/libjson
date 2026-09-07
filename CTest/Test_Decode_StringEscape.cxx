#include <libjson/json.hxx>
#include <sstream>

#include "helpers.hxx"

using libjson::json;

using std::string_literals::operator ""s;

auto Test_Decode_StringEscape(int, char**) -> int {
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

    const auto node {json::decode(ss)};
    helpers::checkEq(node.at("quote"), "\""s);
    helpers::checkEq(node.at("backslash"), "\\"s);
    helpers::checkEq(node.at("forwardSlash"), "/"s);
    helpers::checkEq(node.at("backspace"), "\b"s);
    helpers::checkEq(node.at("formFeed"), "\f"s);
    helpers::checkEq(node.at("newline"), "\n"s);
    helpers::checkEq(node.at("carriageReturn"), "\r"s);
    helpers::checkEq(node.at("tab"), "\t"s);
    helpers::checkEq(node.at("unicode"), "\u03A0"s);

    return 0;
}
