#pragma once

namespace storm {

class FrameClock {
public:
    explicit FrameClock(double max_delta_seconds = 0.1) noexcept;

    double tick(double elapsed_seconds) noexcept;

    double max_delta_seconds() const noexcept;
    void set_max_delta_seconds(double value) noexcept;

private:
    double max_delta_seconds_;
};

} // namespace storm
