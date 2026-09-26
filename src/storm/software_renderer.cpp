#include "storm/render.hpp"

#include <iostream>
#include <memory>

namespace storm {

class SoftwareRenderer final : public Renderer2D {
public:
    bool initialize(int width, int height, const std::string& title) override {
        if (width <= 0 || height <= 0) return false;
        width_ = width;
        height_ = height;
        std::cout << "Storm Renderer: " << title
                  << " (" << width_ << "x" << height_ << ")\n";
        return true;
    }

    void begin_frame(Color clear_color) override {
        std::cout << "frame clear = rgba("
                  << static_cast<int>(clear_color.r) << ","
                  << static_cast<int>(clear_color.g) << ","
                  << static_cast<int>(clear_color.b) << ","
                  << static_cast<int>(clear_color.a) << ")\n";
    }

    void draw_rect(const Rect& rect, Color color) override {
        if (rect.width <= 0.0f || rect.height <= 0.0f) return;
        std::cout << "rect " << rect.x << "," << rect.y
                  << " " << rect.width << "x" << rect.height
                  << " rgba(" << static_cast<int>(color.r) << ","
                  << static_cast<int>(color.g) << ","
                  << static_cast<int>(color.b) << ","
                  << static_cast<int>(color.a) << ")\n";
    }

    void draw_sprite(const Rect& destination, const std::string& texture) override {
        if (destination.width <= 0.0f || destination.height <= 0.0f) return;
        std::cout << "sprite " << texture << " -> "
                  << destination.x << "," << destination.y << " "
                  << destination.width << "x" << destination.height << "\n";
    }

    void end_frame() override {
        std::cout << "frame end\n";
    }

    void shutdown() override {
        width_ = 0;
        height_ = 0;
        std::cout << "Storm Renderer shutdown\n";
    }
};

std::unique_ptr<Renderer2D> create_software_renderer() {
    return std::make_unique<SoftwareRenderer>();
}

} // namespace storm
