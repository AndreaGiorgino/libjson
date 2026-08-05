#pragma once

#include <concepts>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "access_error.hxx"

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
template <typename T>
constexpr bool variant_member_v = variant_member_t<T, value_t>::value;

/*
 * @brief Requires T to be pointer allocated as per value_t_internal definition
 */
template <typename T>
struct ptr_allocated_t
    : std::bool_constant<(std::constructible_from<std::string, T>
                          || std::same_as<std::remove_cvref_t<T>, array_t>
                          || std::same_as<std::remove_cvref_t<T>, object_t>)> {
};

/*
 * @brief Helper method for ptr_allocated_t
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
   public: // ctors
    json(void);

    // ------------ copy ------------

    json(const json& rhs);

    template <typename T>
        requires(variant_member_v<T> && !ptr_allocated_v<T>)
    json(const T& rhs) : _hasValue(true),
                         _value(rhs) {}

    template <typename T>
        requires(ptr_allocated_v<T>)
    json(const T& rhs) : _hasValue(true) {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, T>,
                std::string, std::remove_cvref_t<T>>;

        _value = std::make_unique<target_t>(rhs);
    }

    auto operator =(const json& rhs) -> json&;

    template <typename T>
        requires(variant_member_v<T> && !ptr_allocated_v<T>)
    auto operator =(const T& rhs) -> json& {
        _hasValue = true;
        _value    = rhs;
        return *this;
    }

    template <typename T>
        requires(ptr_allocated_v<T>)
    auto operator =(const T& rhs) -> json& {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, T>,
                std::string, std::remove_cvref_t<T>>;

        _hasValue = true;
        _value    = std::make_unique<target_t>(rhs);
        return *this;
    }

    // ------------------------------

    // ------------ move ------------

    json(json&& rhs);

    template <typename T>
        requires(variant_member_v<T> && !ptr_allocated_v<T>)
    json(T&& rhs) : _hasValue(true),
                    _value(std::forward<T>(rhs)) {}

    template <typename T>
        requires(ptr_allocated_v<T>)
    json(T&& rhs) : _hasValue(true) {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, T>,
                std::string, std::remove_cvref_t<T>>;

        _value = std::make_unique<target_t>(std::forward<T>(rhs));
    }

    auto operator =(json&& rhs) -> json&;

    template <typename T>
        requires(variant_member_v<T> && !ptr_allocated_v<T>)
    auto operator =(T&& rhs) -> json& {
        _hasValue = true;
        _value    = std::forward<T>(rhs);
        return *this;
    }

    template <typename T>
        requires(ptr_allocated_v<T>)
    auto operator =(T&& rhs) -> json& {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, T>,
                std::string, std::remove_cvref_t<T>>;

        _hasValue = true;
        _value    = std::make_unique<target_t>(std::forward<T>(rhs));
        return *this;
    }

    // ------------------------------

    ~json(void) = default;

   public: // methods
    /**
     * @brief Check if a value is being hold
     */
    [[nodiscard]] auto has_value(void) const noexcept -> bool;

    /**
     * @brief Get the stored value as T if possible (primitive)
     *
     * @tparam T The target type
     */
    template <typename T>
        requires(variant_member_v<T> && !ptr_allocated_v<T>)
    [[nodiscard]] auto as(void) const -> std::remove_cvref_t<T> {
        using clean_t = std::remove_cvref_t<T>;

        if (!_hasValue || !std::holds_alternative<clean_t>(_value))
            throw access_error("Value type mismatch");
        return std::get<clean_t>(_value);
    }

    /**
     * @brief Get the stored value as T if possible (pointer allocated)
     *
     * @tparam T The target type
     */
    template <typename T>
        requires ptr_allocated_v<T>
    [[nodiscard]] auto as(void) const -> std::remove_cvref_t<T> {
        using clean_t = std::remove_cvref_t<T>;
        using target_t
            = std::conditional_t<std::constructible_from<std::string, clean_t>,
                std::string, clean_t>;
        using ptr_t = std::unique_ptr<target_t>;

        if (!_hasValue || !std::holds_alternative<ptr_t>(_value))
            throw access_error("Value type mismatch");
        return *std::get<ptr_t>(_value);
    }

    /**
     * @brief Check if a value of type T is being hold
     *
     * @tparam T The type to check
     */
    template <typename T>
        requires variant_member_v<T>
    [[nodiscard]] auto holds_alternative(void) const noexcept -> bool {
        using clean_t = std::remove_cvref_t<T>;

        if constexpr (ptr_allocated_v<clean_t>)
            return std::holds_alternative<std::unique_ptr<clean_t>>(_value);
        else
            return std::holds_alternative<clean_t>(_value);
    }

   private: // definitions
    using value_t_internal
        = std::variant<bool, int, double, std::unique_ptr<std::string>,
            std::unique_ptr<array_t>, std::unique_ptr<object_t>>;

   private: // methods
    static auto _deep_copy(const value_t_internal& val) noexcept -> value_t_internal {
        return std::visit(
            [](const auto& v) noexcept -> value_t_internal {
                using clean_t = std::remove_cvref_t<decltype(v)>;

                // check if v can be dereferenced
                if constexpr (requires(clean_t x) { *x; }) {
                    using element_t =
                        typename clean_t::element_type; // access smart pointer
                                                        // element type
                    using target_t = std::conditional_t<
                        std::constructible_from<std::string, element_t>,
                        std::string, element_t>;

                    return std::make_unique<target_t>(*v);
                } else
                    return v;
            },
            val);
    };

   private: // members
    bool _hasValue {false};
    value_t_internal _value {};
};
} // namespace libjson
