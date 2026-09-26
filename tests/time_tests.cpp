#include "storm/time.hpp"

#include <cassert>
#include <cmath>
#include <limits>

int main() {
    storm::FrameClock clock;
    assert(std::abs(clock.max_delta_seconds() - 0.1) < 1e-12);

    assert(std::abs(clock.tick(0.05) - 0.05) < 1e-12);
    assert(std::abs(clock.tick(1.0) - 0.1) < 1e-12);
    assert(clock.tick(0.0) == 0.0);
    assert(clock.tick(-1.0) == 0.0);
    assert(clock.tick(std::numeric_limits<double>::quiet_NaN()) == 0.0);
    assert(clock.tick(std::numeric_limits<double>::infinity()) == 0.0);

    clock.set_max_delta_seconds(0.25);
    assert(std::abs(clock.max_delta_seconds() - 0.25) < 1e-12);
    assert(std::abs(clock.tick(1.0) - 0.25) < 1e-12);

    clock.set_max_delta_seconds(-1.0);
    assert(clock.max_delta_seconds() == 0.0);
    assert(clock.tick(1.0) == 0.0);

    clock.set_max_delta_seconds(std::numeric_limits<double>::infinity());
    assert(clock.max_delta_seconds() == 0.0);

    return 0;
}
