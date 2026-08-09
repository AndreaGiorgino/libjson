#pragma once

#include <format>
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

/*
 * @brief Default wrapper for variant_member_t
 */
template <typename Tp, typename U>
struct variant_member_t : std::false_type {};

/*
 * @brief Requires Tp to be part of Types
 */
template <typename Tp, typename... Types>
struct variant_member_t<Tp, std::variant<Types...>>
    : std::bool_constant<(
          std::same_as<std::remove_cvref_t<Tp>, Types> || ...)> {};

/*
 * @brief Helper method for variant_member_t
 */
template <typename Tp>
constexpr bool variant_member_v = variant_member_t<Tp, value_t>::value;

/*
 * @brief Requires Tp to be pointer allocated as per value_t_internal definition
 */
template <typename Tp>
struct ptr_allocated_t
    : std::bool_constant<(std::constructible_from<std::string, Tp>
                          || std::same_as<std::remove_cvref_t<Tp>, array_t>
                          || std::same_as<std::remove_cvref_t<Tp>, object_t>)> {
};

/*
 * @brief Helper method for ptr_allocated_t
 */
template <typename Tp>
constexpr bool ptr_allocated_v = ptr_allocated_t<Tp>::value;

// ----------------------------------------------------------------------------

class json final {
   public: // ctors
    json(void) = default;

    // ------------ copy ------------

    json(const json& rhs);

    template <typename Tp>
        requires(variant_member_v<Tp> && !ptr_allocated_v<Tp>)
    explicit json(const Tp& rhs) : _hasValue(true),
                                   _value(rhs) {}

    template <typename Tp>
        requires(ptr_allocated_v<Tp>)
    explicit json(const Tp& rhs) : _hasValue(true) {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, Tp>,
                std::string, std::remove_cvref_t<Tp>>;

        _value = std::make_unique<target_t>(rhs);
    }

    auto operator =(const json& rhs) -> json&;

    template <typename Tp>
        requires(variant_member_v<Tp> && !ptr_allocated_v<Tp>)
    auto operator =(const Tp& rhs) -> json& {
        _hasValue = true;
        _value    = rhs;
        return *this;
    }

    template <typename Tp>
        requires(ptr_allocated_v<Tp>)
    auto operator =(const Tp& rhs) -> json& {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, Tp>,
                std::string, std::remove_cvref_t<Tp>>;

        _hasValue = true;
        _value    = std::make_unique<target_t>(rhs);
        return *this;
    }

    // ------------------------------

    // ------------ move ------------

    json(json&& rhs);

    template <typename Tp>
        requires(variant_member_v<Tp> && !ptr_allocated_v<Tp>)
    json(Tp&& rhs) : _hasValue(true),
                     _value(std::forward<Tp>(rhs)) {}

    template <typename Tp>
        requires(ptr_allocated_v<Tp>)
    json(Tp&& rhs) : _hasValue(true) {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, Tp>,
                std::string, std::remove_cvref_t<Tp>>;

        _value = std::make_unique<target_t>(std::forward<Tp>(rhs));
    }

    auto operator =(json&& rhs) -> json&;

    template <typename Tp>
        requires(variant_member_v<Tp> && !ptr_allocated_v<Tp>)
    auto operator =(Tp&& rhs) -> json& {
        _hasValue = true;
        _value    = std::forward<Tp>(rhs);
        return *this;
    }

    template <typename Tp>
        requires(ptr_allocated_v<Tp>)
    auto operator =(Tp&& rhs) -> json& {
        using target_t
            = std::conditional_t<std::constructible_from<std::string, Tp>,
                std::string, std::remove_cvref_t<Tp>>;

        _hasValue = true;
        _value    = std::make_unique<target_t>(std::forward<Tp>(rhs));
        return *this;
    }

    // ------------------------------

    ~json(void) = default;

   public: // methods
    /**
     * @brief Check if a value is being stored
     * @return The result of the check
     */
    [[nodiscard]] auto has_value(void) const noexcept -> bool;

    /**
     * @brief Get the stored value as Tp if possible (primitive)
     *
     * @tparam Tp The target type
     * @return The stored value of type Tp
     *
     * @throws std::bad_variant_access If the stored value is not of type Tp
     */
    template <typename Tp>
        requires(variant_member_v<Tp> && !ptr_allocated_v<Tp>)
    [[nodiscard]] auto as(void) const -> std::remove_cvref_t<Tp> {
        using clean_t = std::remove_cvref_t<Tp>;

        if (!_hasValue || !std::holds_alternative<clean_t>(_value))
            throw std::bad_variant_access();
        return std::get<clean_t>(_value);
    }

    /**
     * @brief Get the stored value as Tp if possible (pointer allocated)
     *
     * @tparam Tp The target type
     * @return The stored value of type Tp
     *
     * @throws std::bad_variant_access If the stored value is not of type Tp
     */
    template <typename Tp>
        requires ptr_allocated_v<Tp>
    [[nodiscard]] auto as(void) const -> std::remove_cvref_t<Tp> {
        using clean_t = std::remove_cvref_t<Tp>;
        using target_t
            = std::conditional_t<std::constructible_from<std::string, clean_t>,
                std::string, clean_t>;
        using ptr_t = std::unique_ptr<target_t>;

        if (!_hasValue || !std::holds_alternative<ptr_t>(_value))
            throw std::bad_variant_access();
        return *std::get<ptr_t>(_value);
    }

    /**
     * @brief Check if a value of type Tp is being hold
     *
     * @tparam Tp The type to check
     * @return The result of the check
     */
    template <typename Tp>
        requires variant_member_v<Tp>
    [[nodiscard]] auto holds_alternative(void) const noexcept -> bool {
        using clean_t = std::remove_cvref_t<Tp>;

        if constexpr (ptr_allocated_v<clean_t>)
            return std::holds_alternative<std::unique_ptr<clean_t>>(_value);
        else
            return std::holds_alternative<clean_t>(_value);
    }

    /**
     * @brief Get a reference to the element in the stored array at the specified
     * location
     *
     * @param index The position of the element
     * @return The reference to the element
     *
     * @throws std::runtime_error If no value is being stored
     * @throws std::bad_variant_access If the stored value is not an array
     * @throws std::out_of_range If the location exceeds the bounds of the array
     */
    [[nodiscard]] auto at(std::size_t index) const -> const json& {
        if (!_hasValue)
            throw std::runtime_error("No value is being stored");
        else
            return std::get<array_ptr_t>(_value)->at(index);
    }

    /**
     * @brief Get a reference to the element in the stored array at the specified
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
        else
            return std::get<array_ptr_t>(_value)->at(index);
    }

    /**
     * @brief Copy data to the end of the stored array
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
                    using Tp = decltype(val);

                    if constexpr (requires(Tp x) { *x; })
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
     * @brief Move data to the end of the stored array
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
                    using Tp = decltype(val);

                    if constexpr (requires(Tp x) { *x; })
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
     * @brief Get a reference to the element in the stored object with the
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
    [[nodiscard]] auto at(std::string_view key) const -> const json& {
        if (!_hasValue)
            throw std::runtime_error("No value is being stored");
        else
            return std::get<object_ptr_t>(_value)->at(std::string {key});
    }

    /**
     * @brief Get a reference to the element in the stored object with the
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

    /**
     * @brief Copy data to the stored object
     *
     * @param key The key
     * @param val The data to add
     *
     * @throws std::runtime_error If the key already exists
     * @throws std::bad_variant_access If the stored value is not an object
     */
    auto insert(std::string_view key, const json& val) -> void {
        if (!_hasValue) {
            _hasValue = true;
            _value    = std::make_unique<object_t>(object_t {});
        }

        const std::string str {key};
        auto& ptr {std::get<object_ptr_t>(_value)};

        if (ptr->contains(str))
            throw std::runtime_error(
                std::format("Key already present in object: {}", key));
        ptr->insert({str, val});
    }

    /**
     * @brief Move data to the stored object
     *
     * @param key The key
     * @param val The data to add
     *
     * @throws std::runtime_error If the key already exists
     * @throws std::bad_variant_access If the stored value is not an object
     */
    auto insert(std::string_view key, json&& val) -> void {
        if (!_hasValue) {
            _hasValue = true;
            _value    = std::make_unique<object_t>(object_t {});
        }

        const std::string str {key};
        auto& ptr {std::get<object_ptr_t>(_value)};

        if (ptr->contains(str))
            throw std::runtime_error(
                std::format("Key already present in object: {}", key));
        ptr->insert({str, std::move(val)});
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
