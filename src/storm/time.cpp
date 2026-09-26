#include "storm/time.hpp"

#include <algorithm>
#include <cmath>

namespace storm {

FrameClock::FrameClock(double max_delta_seconds) noexcept
    : max_delta_seconds_(std::max(
          0.0, std::isfinite(max_delta_seconds) ? max_delta_seconds : 0.0)) {}

double FrameClock::tick(double elapsed_seconds) noexcept {
    if (!std::isfinite(elapsed_seconds) || elapsed_seconds <= 0.0) {
        return 0.0;
    }

    return std::clamp(elapsed_seconds, 0.0, max_delta_seconds_);
}

double FrameClock::max_delta_seconds() const noexcept {
    return max_delta_seconds_;
}

void FrameClock::set_max_delta_seconds(double value) noexcept {
    max_delta_seconds_ = std::max(
        0.0, std::isfinite(value) ? value : 0.0);
}

} // namespace storm
