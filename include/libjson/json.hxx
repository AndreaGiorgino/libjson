#pragma once

#include <concepts>
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

// ----------------------------------------------------------------------------

class json final {
   public: // ctors
    json(void) = default;

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
     * @return The result of the check
     */
    [[nodiscard]] auto has_value(void) const noexcept -> bool;

    /**
     * @brief Get the stored value as T if possible (primitive)
     *
     * @tparam T The target type
     * @return The stored value of type T
     *
     * @throws std::bad_variant_access If the stored value is not of type T
     */
    template <typename T>
        requires(variant_member_v<T> && !ptr_allocated_v<T>)
    [[nodiscard]] auto as(void) const -> std::remove_cvref_t<T> {
        using clean_t = std::remove_cvref_t<T>;

        if (!_hasValue || !std::holds_alternative<clean_t>(_value))
            throw std::bad_variant_access();
        return std::get<clean_t>(_value);
    }

    /**
     * @brief Get the stored value as T if possible (pointer allocated)
     *
     * @tparam T The target type
     * @return The stored value of type T
     *
     * @throws std::bad_variant_access If the stored value is not of type T
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
            throw std::bad_variant_access();
        return *std::get<ptr_t>(_value);
    }

    /**
     * @brief Check if a value of type T is being hold
     *
     * @tparam T The type to check
     * @return The result of the check
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

    /**
     * @brief Returns a reference to the element in the array at the specified
     * location
     *
     * @param index The position of the element
     * @return The reference to the element
     *
     * @throws std::runtime_error If no value is being stored
     * @throws std::bad_variant_access If the stored value is not an array
     * @throws std::out_of_range If the location exceeds the bounds of the array
     */
    template <typename Self>
    [[nodiscard]] auto at(this Self&& self, std::size_t index) -> Self {
        if (!self._hasValue)
            throw std::runtime_error("No value is being stored");

        return std::get<array_ptr_t>(std::forward<Self>(self)._value)
            ->at(index);
    }

    /**
     * @brief Returns a reference to the element in the array at the specified
     * location
     *
     * @param index The position of the element
     * @return The reference to the element
     *
     * @throws std::runtime_error If no value is being stored
     * @throws std::bad_variant_access If the stored value is not an array
     * @throws std::out_of_range If the location exceeds the bounds of the array
     */
    [[nodiscard]] auto operator [](std::size_t index) -> json& {
        if (!_hasValue)
            throw std::runtime_error("No value is being stored");

        return std::get<array_ptr_t>(_value)->at(index);
    }

    /**
     * @brief Add data to the end of the array
     *
     * @param val The data to add
     */
    auto push_back(const json& val) -> void {
        if (!_hasValue) {
            _hasValue = true;
            _value    = std::make_unique<array_t>(array_t {});
        }

        if (!holds_alternative<array_t>()) {
            auto buffer {std::visit(
                [](auto& val) -> json {
                    using T = decltype(val);

                    if constexpr (requires(T x) { *x; })
                        return {*val};
                    else
                        return {val};
                },
                _value)};

            _value = std::make_unique<array_t>(array_t {std::move(buffer)});
        }

        std::get<array_ptr_t>(_value)->push_back(val);
    }

    /**
     * @brief Add data to the end of the array
     *
     * @param val The data to add
     */
    auto push_back(json&& val) -> void {
        if (!_hasValue) {
            _hasValue = true;
            _value    = std::make_unique<array_t>(array_t {});
        }

        if (!holds_alternative<array_t>()) {
            auto buffer {std::visit(
                [](auto& val) -> json {
                    using T = decltype(val);

                    if constexpr (requires(T x) { *x; })
                        return {*val};
                    else
                        return {val};
                },
                _value)};

            _value = std::make_unique<array_t>(array_t {std::move(buffer)});
        }

        std::get<array_ptr_t>(_value)->push_back(std::move(val));
    }

    /**
     * @brief Returns a reference to the element in the object with the
     * specified key
     *
     * @param key The key of the element
     * @return The reference to the element
     *
     * @throws std::runtime_error If no value is being stored
     * @throws std::bad_variant_access If the stored value is not an object
     * @throws std::out_of_range If the object does not have an element with the
     * specified key
     */
    template <typename Self>
    [[nodiscard]] auto at(this Self&& self, std::string_view key) -> Self {
        if (!self._hasValue)
            throw std::runtime_error("No value is being stored");

        return std::get<object_ptr_t>(self._value)->at(std::string {key});
    }

    /**
     * @brief Returns a reference to the element in the object with the
     * specified key. Insert it if it does not exist
     *
     * @param key The key of the element
     * @return The reference to the element
     *
     * @throws std::runtime_error If no value is being stored
     * @throws std::bad_variant_access If the stored value is not an object
     * @throws std::out_of_range If the object does not have an element with the
     * specified key
     */
    [[nodiscard]] auto operator [](std::string_view key) -> json& {
        if (!_hasValue) {
            _hasValue = true;
            _value    = std::make_unique<object_t>(object_t {});
        }

        return (*std::get<object_ptr_t>(_value))[std::string {key}];
    }

   private: // definitions
    /*
     * @brief Represents a pointer to an array_t
     */
    using array_ptr_t = std::unique_ptr<array_t>;

    /*
     * @brief Represents a pointer to an object_t
     */
    using object_ptr_t = std::unique_ptr<object_t>;

    /*
     * @brief Represents a json value internal implementation
     */
    using value_t_internal = std::variant<bool, int, double,
        std::unique_ptr<std::string>, array_ptr_t, object_ptr_t>;

   private: // methods
    /**
     * @brief Deep copy the stored value
     *
     * @param val The variant to copy from
     * @return The variant copied value
     */
    static auto _deep_copy(const value_t_internal& val) noexcept
        -> value_t_internal {
        return std::visit(
            [](const auto& val) noexcept -> value_t_internal {
                using clean_t = std::remove_cvref_t<decltype(val)>;

                if constexpr (requires(clean_t x) { *x; }) {
                    using element_t = typename clean_t::element_type;
                    using target_t  = std::conditional_t<
                        std::constructible_from<std::string, element_t>,
                        std::string, element_t>;

                    return std::make_unique<target_t>(*val);
                } else
                    return val;
            },
            val);
    };

   private: // members
    bool _hasValue {false};
    value_t_internal _value {};
};
} // namespace libjson
