#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace storm {

struct Color {
    std::uint8_t r{255};
    std::uint8_t g{255};
    std::uint8_t b{255};
    std::uint8_t a{255};
};

class Texture2D {
public:
    Texture2D() = default;
    Texture2D(int width, int height, std::vector<Color> pixels);

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    const std::vector<Color>& pixels() const noexcept { return pixels_; }
    bool valid() const noexcept;

private:
    int width_{0};
    int height_{0};
    std::vector<Color> pixels_;
};

struct Rect {
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
};

class Renderer2D {
public:
    virtual ~Renderer2D() = default;

    virtual bool initialize(int width, int height, const std::string& title) = 0;
    virtual void begin_frame(Color clear_color) = 0;
    virtual void draw_rect(const Rect& rect, Color color) = 0;
    virtual void draw_sprite(const Rect& destination, const Texture2D& texture) = 0;
    virtual void end_frame() = 0;
    virtual void shutdown() = 0;

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }

protected:
    int width_{0};
    int height_{0};
};

std::unique_ptr<Renderer2D> create_software_renderer();

} // namespace storm
