#pragma once

#include "storm/input.hpp"

#include <memory>

namespace storm {

using PlatformInputState = InputState;

class Platform {
public:
    virtual ~Platform() = default;

    virtual bool initialize(int width, int height, const char* title) = 0;
    virtual bool poll_events(InputState& input) = 0;
    virtual void shutdown() = 0;

    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
};

std::unique_ptr<Platform> create_headless_platform();

} // namespace storm
