#pragma once
#include "dans/development_markers.hpp"
#include "dans/types.hpp"

#include <chrono>
#include <print>

namespace dans::chrono
{
using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;

using Duration = std::chrono::duration<f64>;
using std::chrono::duration_cast;

[[nodiscard]] def format_seconds(Duration dur, bool show_milliseconds = false) -> std::string
{
    const auto is_negative = dur < std::chrono::duration<f64>::zero();
    const auto positive_duration = is_negative ? -dur : dur;

    using Msec = std::chrono::milliseconds;
    constexpr Msec::rep ms_per_second{1000};
    constexpr Msec::rep ms_per_minute{ms_per_second * 60};
    constexpr Msec::rep ms_per_hour{ms_per_minute * 60};
    constexpr Msec::rep ms_per_day{ms_per_hour * 24};

    auto remaining = duration_cast<Msec>(positive_duration).count();

    const auto days = remaining / ms_per_day;
    remaining %= ms_per_day;
    const auto hours = remaining / ms_per_hour;
    remaining %= ms_per_hour;
    const auto minutes = remaining / ms_per_minute;
    remaining %= ms_per_minute;
    const auto seconds = remaining / ms_per_second;
    const auto milliseconds = remaining % ms_per_second;

    std::string result{};
    auto started = false;

    const auto append_time_unit = [&](Msec::rep value, std::string_view name) -> void
    {
        if (not started and value == 0) return;
        if (not result.empty()) result += ' ';
        result += std::format("{} {}{}", value, name, value == 1 ? "" : "s");
        started = true;
    };
    append_time_unit(days, "day");
    append_time_unit(hours, "hour");
    append_time_unit(minutes, "minute");
    append_time_unit(seconds, "second");
    if (show_milliseconds) append_time_unit(milliseconds, "millisecond");

    if (not started) result = show_milliseconds ? "0 milliseconds" : "0 seconds";

    if (is_negative) return std::format("-{}", result);
    return result;
}

class ScopeTimer
{

  public:
    explicit ScopeTimer(std::string_view name, bool print_millis = false)
        : name_{name}, start_{Clock::now()}, print_millis_{print_millis}
    {
    }
    ~ScopeTimer()
    {
        try
        {
            const std::chrono::duration<f64> dur{Clock::now() - start_};
            std::println("{} finished in {}.", name_, format_seconds(dur, print_millis_));
        }
        catch (...)  // NOLINT
        {
        }
    }
    ScopeTimer(const ScopeTimer&) = delete;
    ScopeTimer(ScopeTimer&&) = delete;
    def operator=(const ScopeTimer&)->ScopeTimer& = delete;
    def operator=(ScopeTimer&&)->ScopeTimer& = delete;

  private:
    std::string_view name_{};
    TimePoint start_{};
    bool print_millis_{false};
};

class StopWatch
{
    static constexpr usize k_initial_capacity{4};

  public:
    StopWatch()
    {
        durations_.reserve(k_initial_capacity);
    }

    def start() -> void
    {
        if (running_) throw std::runtime_error("StopWatch::start() called while already running");
        start_time_ = Clock::now();
        running_ = true;
    }

    def stop() -> void
    {
        if (not running_)
            throw std::runtime_error("StopWatch::stop() called without prior start()");
        const std::chrono::duration<f64> elapsed{Clock::now() - start_time_};
        durations_.push_back(elapsed.count());
        running_ = false;
    }

    def abort() -> void
    {
        running_ = false;
    }

    def clear() -> void
    {
        durations_.clear();
        running_ = false;
    }

    [[nodiscard]] def durations() const noexcept -> const std::vector<f64>&
    {
        return durations_;
    }

    [[nodiscard]] def is_running() const -> bool
    {
        return running_;
    }

  private:
    std::vector<f64> durations_{};
    TimePoint start_time_{};
    bool running_{false};
};
}  // namespace dans::chrono

#define DANS_DETAIL_CONCAT_(a, b) a##b
#define DANS_DETAIL_CONCAT(a, b) DANS_DETAIL_CONCAT_(a, b)

#define DANS_SCOPE_TIMER(msg)                                                                      \
    std::println("[{}:{}] {} started.", __FILE__, __LINE__, (msg));                                \
    const ::dans::chrono::ScopeTimer DANS_DETAIL_CONCAT(dans_scope_timer_, __LINE__)               \
    {                                                                                              \
        (msg)                                                                                      \
    }
