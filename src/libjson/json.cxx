#include "libjson/json.hxx"

namespace libjson {
json::json(const json& rhs)
    : _hasValue(rhs._hasValue),
      _value(_deep_copy(rhs._value)) {}

auto json::operator =(const json& rhs) -> json& {
    if (this != &rhs) {
        _hasValue = rhs._hasValue;
        _value    = _deep_copy(rhs._value);
    }

    return *this;
}

json::json(json&& rhs)
    : _hasValue(rhs._hasValue),
      _value(std::move(rhs._value)) {
    rhs._hasValue = false;
}

auto json::operator =(json&& rhs) -> json& {
    _hasValue     = rhs._hasValue;
    _value        = std::move(rhs._value);
    rhs._hasValue = false;
    return *this;
}

auto json::has_value(void) const noexcept -> bool {
    return _hasValue;
}
} // namespace libjson
