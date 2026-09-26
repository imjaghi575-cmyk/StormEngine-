#include "storm/fixed_timestep.hpp"

#include <cassert>
#include <cmath>
#include <limits>

int main() {
    storm::FixedTimestep clock(0.1, 3);

    int steps = 0;
    double sum = 0.0;

    assert(clock.advance(0.05, [&](double dt) {
        ++steps;
        sum += dt;
    }) == 0);
    assert(steps == 0);

    assert(clock.advance(0.16, [&](double dt) {
        ++steps;
        sum += dt;
    }) == 2);
    assert(steps == 2);
    assert(std::abs(sum - 0.2) < 1e-12);

    assert(clock.advance(0.35, [&](double dt) {
        ++steps;
        sum += dt;
    }) == 3);
    assert(steps == 5);
    assert(std::abs(clock.accumulator() - 0.01) < 1e-12);

    const int before = steps;
    assert(clock.advance(-1.0, [&](double) { ++steps; }) == 0);
    assert(clock.advance(std::numeric_limits<double>::quiet_NaN(),
                         [&](double) { ++steps; }) == 0);
    assert(steps == before);

    clock.set_step(0.0);
    assert(clock.step() == 0.1);
    clock.set_max_steps_per_tick(0);
    assert(clock.max_steps_per_tick() == 3);

    return 0;
}
