#pragma once

#include "storm/render.hpp"

#include <string>
#include <vector>

namespace storm {

class SoftwareRenderer final : public Renderer2D {
public:
    bool initialize(int width, int height, const std::string& title) override;
    void begin_frame(Color clear_color) override;
    void draw_rect(const Rect& rect, Color color) override;
    void draw_sprite(const Rect& destination,
                     const Texture2D& texture) override;
    void end_frame() override;
    void shutdown() override;

    const std::vector<Color>& framebuffer() const noexcept;

private:
    static bool valid_rect(const Rect& rect) noexcept;

    bool initialized_{false};
    std::vector<Color> pixels_;
};

} // namespace storm
