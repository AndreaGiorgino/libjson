#pragma once

#include <istream>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

namespace libjson {
/**
 * @class json
 * @brief Represents a json element
 *
 */
class json;

/**
 * @brief Represents a json array value
 */
using array_t = std::vector<json>;

/**
 * @brief Represents a json object value
 */
using object_t = std::unordered_map<std::string, json>;

/**
 * @brief Represents a json value
 */
using value_t = std::variant<bool, int, double, std::string, array_t, object_t>;


/**
 * @brief Load a json from raw string
 *
 * @param raw The input raw json
 */
auto load(std::string_view raw) -> json;

/**
 * @brief Load a json from stream
 *
 * @param is The input stream
 */
auto load(std::istream&& is) -> json;

// ----------------------------------------------------------------------------

class json final {
   public:
   public: // ctors
    json(void) = default;

    json(const json&)                     = default;
    auto operator =(const json&) -> json& = default;

    json(json&&)                     = default;
    auto operator =(json&&) -> json& = default;

    ~json(void) = default;

   public: // methods
    /**
     * @brief Get if a value is being hold
     */
    [[nodiscard]] auto has_value(void) const noexcept -> bool;

    /**
     * @brief Get if a value of type Tp is being hold
     *
     * @tparam Tp The type to check
     */
    template <typename Tp>
    [[nodiscard]] auto holds_alternative(void) const noexcept -> bool {
        return std::holds_alternative<Tp>(_value);
    }

   private: // definitions
    using value_t_internal
        = std::variant<bool, int, double, std::unique_ptr<std::string>,
            std::unique_ptr<array_t>, std::unique_ptr<object_t>>;

   private: // members
    bool _hasValue {false};
    value_t_internal _value {};
};
} // namespace libjson
