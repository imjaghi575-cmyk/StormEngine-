#include "storm/render.hpp"

#include <iostream>

namespace storm {

class SoftwareRenderer final : public Renderer2D {
public:
    bool initialize(int width, int height, const std::string& title) override {
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
        std::cout << "rect " << rect.x << "," << rect.y
                  << " " << rect.width << "x" << rect.height
                  << " rgba(" << static_cast<int>(color.r) << ","
                  << static_cast<int>(color.g) << ","
                  << static_cast<int>(color.b) << ","
                  << static_cast<int>(color.a) << ")\n";
    }

    void draw_sprite(const Rect& destination, const std::string& texture) override {
        std::cout << "sprite " << texture << " -> "
                  << destination.x << "," << destination.y << " "
                  << destination.width << "x" << destination.height << "\n";
    }

    void end_frame() override {
        std::cout << "frame end\n";
    }

    void shutdown() override {
        std::cout << "Storm Renderer shutdown\n";
    }
};

} // namespace storm
