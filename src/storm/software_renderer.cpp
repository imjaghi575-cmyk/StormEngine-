#include "storm/render.hpp"

#include <cmath>
#include <iostream>
#include <memory>

namespace storm {

class SoftwareRenderer final : public Renderer2D {
public:
    bool initialize(int width, int height, const std::string& title) override {
        if (width <= 0 || height <= 0) {
            shutdown();
            return false;
        }

        width_ = width;
        height_ = height;
        initialized_ = true;

        std::cout << "Storm Renderer: " << title
                  << " (" << width_ << "x" << height_ << ")\n";
        return true;
    }

    void begin_frame(Color clear_color) override {
        if (!initialized_) return;

        std::cout << "frame clear = rgba("
                  << static_cast<int>(clear_color.r) << ","
                  << static_cast<int>(clear_color.g) << ","
                  << static_cast<int>(clear_color.b) << ","
                  << static_cast<int>(clear_color.a) << ")\n";
    }

    void draw_rect(const Rect& rect, Color color) override {
        if (!initialized_ || !valid_rect(rect)) return;

        std::cout << "rect " << rect.x << "," << rect.y
                  << " " << rect.width << "x" << rect.height
                  << " rgba(" << static_cast<int>(color.r) << ","
                  << static_cast<int>(color.g) << ","
                  << static_cast<int>(color.b) << ","
                  << static_cast<int>(color.a) << ")\n";
    }

    void draw_sprite(const Rect& destination, const std::string& texture) override {
        if (!initialized_ || !valid_rect(destination) || texture.empty()) return;

        std::cout << "sprite " << texture << " -> "
                  << destination.x << "," << destination.y << " "
                  << destination.width << "x" << destination.height << "\n";
    }

    void end_frame() override {
        if (!initialized_) return;
        std::cout << "frame end\n";
    }

    void shutdown() override {
        initialized_ = false;
        width_ = 0;
        height_ = 0;
    }

private:
    static bool valid_rect(const Rect& rect) noexcept {
        return std::isfinite(rect.x) && std::isfinite(rect.y) &&
               std::isfinite(rect.width) && std::isfinite(rect.height) &&
               rect.width > 0.0f && rect.height > 0.0f;
    }

    bool initialized_{false};
};

std::unique_ptr<Renderer2D> create_software_renderer() {
    return std::make_unique<SoftwareRenderer>();
}

} // namespace storm
