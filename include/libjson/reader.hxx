#pragma once

namespace libjson {
class reader final {
   public: // definitions
   public: // ctors
    reader(void) = default;

    reader(const reader&)                     = default;
    auto operator =(const reader&) -> reader& = default;

    reader(reader&&)                     = default;
    auto operator =(reader&&) -> reader& = default;

    ~reader(void) = default;

   public:  // methods
   private: // methods
   private: // members
};
} // namespace libjson
