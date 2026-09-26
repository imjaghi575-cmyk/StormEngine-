#pragma once

#include <cmath>

namespace storm {

class FixedTimestep {
public:
    explicit FixedTimestep(double step_seconds = 1.0 / 60.0,
                           int max_steps_per_tick = 8) noexcept;

    void set_step(double step_seconds) noexcept;
    double step() const noexcept;

    void set_max_steps_per_tick(int value) noexcept;
    int max_steps_per_tick() const noexcept;

    template <typename UpdateFn>
    int advance(double elapsed_seconds, UpdateFn&& update) {
        if (!is_valid_elapsed(elapsed_seconds)) {
            return 0;
        }

        accumulator_ += elapsed_seconds;
        if (!std::isfinite(accumulator_)) {
            accumulator_ = 0.0;
            return 0;
        }

        int steps = 0;
        while (accumulator_ >= step_seconds_ &&
               steps < max_steps_per_tick_) {
            update(step_seconds_);
            accumulator_ -= step_seconds_;
            ++steps;
        }

        if (steps == max_steps_per_tick_ && accumulator_ >= step_seconds_) {
            accumulator_ = std::fmod(accumulator_, step_seconds_);
        }

        return steps;
    }

    double accumulator() const noexcept;

private:
    static bool is_valid_elapsed(double value) noexcept;

    double step_seconds_;
    int max_steps_per_tick_;
    double accumulator_{0.0};
};

} // namespace storm
