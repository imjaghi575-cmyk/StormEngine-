#include "storm/platform.hpp"

#include <memory>

namespace storm {

class HeadlessPlatform final : public Platform {
public:
    bool initialize(int width, int height, const char*) override {
        if (width <= 0 || height <= 0) {
            shutdown();
            return false;
        }

        width_ = width;
        height_ = height;
        initialized_ = true;
        return true;
    }

    bool poll_events(InputState&) override {
        return initialized_;
    }

    void shutdown() override {
        initialized_ = false;
        width_ = 0;
        height_ = 0;
    }

    int width() const noexcept override { return width_; }
    int height() const noexcept override { return height_; }

private:
    int width_{0};
    int height_{0};
    bool initialized_{false};
};

std::unique_ptr<Platform> create_headless_platform() {
    return std::make_unique<HeadlessPlatform>();
}

} // namespace storm
