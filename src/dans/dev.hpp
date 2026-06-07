// src/dans/dev.hpp
#pragma once
// Externals
#include <dans/development_markers.hpp>
// StdLib
#include <utility>
//

namespace dans::dev
{
template <typename F>
class Defer
{
  public:
    explicit Defer(F fn) : fn_{std::move(fn)}
    {
    }
    ~Defer()
    {
        try
        {
            fn_();
        }
        catch (...)  // NOLINT
        {
        }
    }
    Defer(const Defer&) = delete;
    Defer(Defer&&) = delete;
    def operator=(const Defer&) -> Defer& = delete;
    def operator=(Defer&&) -> Defer& = delete;

  private:
    F fn_;
};
template <typename F>
Defer(F) -> Defer<F>;
}  // namespace dans::dev

#define DANS_DETAIL_DEFER_CONCAT_(a, b) a##b
#define DANS_DETAIL_DEFER_CONCAT(a, b) DANS_DETAIL_DEFER_CONCAT_(a, b)

#define DANS_DEFER(...) \
    const ::dans::dev::Defer DANS_DETAIL_DEFER_CONCAT(dans_defer_, __LINE__){__VA_ARGS__}
