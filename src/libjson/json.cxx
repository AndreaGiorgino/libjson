#include "libjson/json.hxx"

namespace libjson {
/**
 * @brief Trim left string
 *
 * @param str The string to trim
 */
auto ltrim(std::string str) noexcept -> std::string;

// ----------------------------------------------------------------------------

auto ltrim(std::string str) noexcept -> std::string {
    str.erase(
        str.begin(), std::find_if(str.begin(), str.end(),
                         [](const auto ch) { return !std::isspace(ch); }));
    return str;
}

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
auto json::_deep_copy(const value_t_internal& val) noexcept
    -> value_t_internal {
    return std::visit(
        [](const auto& v) noexcept -> value_t_internal {
            using clean_t = std::remove_cvref_t<decltype(v)>;

            if constexpr (requires(clean_t x) { *x; }) {
                using element_t = typename clean_t::element_type;
                using target_t  = std::conditional_t<
                    std::constructible_from<std::string, element_t>,
                    std::string, element_t>;

                return std::make_unique<target_t>(*v);
            } else
                return v;
        },
        val);
};
} // namespace libjson
