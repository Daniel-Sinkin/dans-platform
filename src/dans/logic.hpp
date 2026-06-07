#pragma once
#include "dans/types.hpp"

namespace dans::logic
{
[[nodiscard]] auto implies(bool a, bool b) noexcept -> bool
{
    return (not a) or b;
}
}  // namespace dans::logic
