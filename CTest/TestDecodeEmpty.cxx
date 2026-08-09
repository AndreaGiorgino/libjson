#include <sstream>

#include "libjson/extra/decode.hxx"

auto TestDecodeEmpty(int, char**) -> int {
    std::stringstream ss {};

    const auto el {libjson::decode(ss)};
    if (el.has_value()) throw std::runtime_error("Unexpected content");

    return 0;
}
