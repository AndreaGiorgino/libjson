#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
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

/*
 * @brief Default wrapper for variant_member_t
 */
template <typename T, typename U>
struct variant_member_t : std::false_type {};

/*
 * @brief Requires T to be part of Types
 */
template <typename T, typename... Types>
struct variant_member_t<T, std::variant<Types...>>
    : std::bool_constant<(std::same_as<std::remove_cvref_t<T>, Types> || ...)> {
};

/*
 * @brief Helper method for variant_member_t
 */
template <typename T, typename U>
constexpr bool variant_member_v = variant_member_t<T, U>::value;

/*
 * @brief Requires T to be pointer allocated as per value_t_internal definition
 */
template <typename T>
struct ptr_allocated_t
    : std::bool_constant<(std::constructible_from<std::string, T>
                          || std::same_as<std::remove_cvref<T>, array_t>
                          || std::same_as<std::remove_cvref<T>, object_t>)> {};

/*
 * @brief Helper method for ptr_allocated_v
 */
template <typename T>
constexpr bool ptr_allocated_v = ptr_allocated_t<T>::value;

/**
 * @brief Decode json from raw string
 *
 * @param raw The input raw json
 */
auto decode(std::string_view raw) -> json;

/**
 * @brief Decode json from stream
 *
 * @param is The input stream
 */
auto decode(std::istream&& is) -> json;

/**
 * @brief Encode json into string
 *
 * @param el The json element
 */
auto encode(json&& el) -> std::string;

/**
 * @brief Encode json into stream
 *
 * @param el The json element
 * @param os The output stream
 */
auto encode(json&& el, std::ostream& os) -> void;

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
