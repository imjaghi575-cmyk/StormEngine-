#include "storm/fixed_timestep.hpp"

#include <algorithm>
#include <cmath>

namespace storm {

FixedTimestep::FixedTimestep(double step_seconds,
                             int max_steps_per_tick) noexcept
    : step_seconds_(1.0 / 60.0),
      max_steps_per_tick_(8) {
    set_step(step_seconds);
    set_max_steps_per_tick(max_steps_per_tick);
}

void FixedTimestep::set_step(double step_seconds) noexcept {
    if (std::isfinite(step_seconds) && step_seconds > 0.0) {
        step_seconds_ = step_seconds;
    }
}

double FixedTimestep::step() const noexcept {
    return step_seconds_;
}

void FixedTimestep::set_max_steps_per_tick(int value) noexcept {
    if (value > 0) {
        max_steps_per_tick_ = value;
    }
}

int FixedTimestep::max_steps_per_tick() const noexcept {
    return max_steps_per_tick_;
}

double FixedTimestep::accumulator() const noexcept {
    return accumulator_;
}

bool FixedTimestep::is_valid_elapsed(double value) noexcept {
    return std::isfinite(value) && value >= 0.0;
}

} // namespace storm
