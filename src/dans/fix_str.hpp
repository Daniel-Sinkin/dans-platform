#pragma once
#include "dans/types.hpp"
//
#include <array>
#include <string>

namespace dans
{
template <usize N>
class FString
{
  public:
    [[nodiscard]] auto to_string() -> std::string
    {
        // std::string tries to initialiser list and b.c. chars are
        // implicitly convertible to int that as potential to
        // to cause problems so just using brace init and NRVO here.
        auto result = std::string(reinterpret_cast<const char*>(data_.data()), N);
        return result;
    }

  private:
    std::array<c8, N> data_{};
};
}  // namespace dans
