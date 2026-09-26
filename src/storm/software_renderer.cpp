#include "storm/software_renderer.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>

namespace storm {

bool SoftwareRenderer::initialize(int width, int height,
                                  const std::string& title) {
    if (width <= 0 || height <= 0) {
        shutdown();
        return false;
    }

    const auto pixel_count = static_cast<std::size_t>(width) *
                             static_cast<std::size_t>(height);
    try {
        pixels_.assign(pixel_count, Color{});
    } catch (...) {
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

void SoftwareRenderer::begin_frame(Color clear_color) {
    if (!initialized_) return;
    for (auto& pixel : pixels_) pixel = clear_color;
}

void SoftwareRenderer::draw_rect(const Rect& rect, Color color) {
    if (!initialized_ || !valid_rect(rect)) return;

    const int left = std::max(0, static_cast<int>(std::floor(rect.x)));
    const int top = std::max(0, static_cast<int>(std::floor(rect.y)));
    const int right = std::min(
        width_, static_cast<int>(std::ceil(rect.x + rect.width)));
    const int bottom = std::min(
        height_, static_cast<int>(std::ceil(rect.y + rect.height)));

    for (int y = top; y < bottom; ++y) {
        for (int x = left; x < right; ++x) {
            pixels_[static_cast<std::size_t>(y) *
                        static_cast<std::size_t>(width_) +
                    static_cast<std::size_t>(x)] = color;
        }
    }
}

void SoftwareRenderer::draw_sprite(const Rect& destination,
                                    const std::string& texture) {
    if (!initialized_ || !valid_rect(destination) || texture.empty()) return;
}

void SoftwareRenderer::end_frame() {
    if (!initialized_) return;
}

void SoftwareRenderer::shutdown() {
    initialized_ = false;
    pixels_.clear();
    width_ = 0;
    height_ = 0;
}

const std::vector<Color>& SoftwareRenderer::framebuffer() const noexcept {
    return pixels_;
}

bool SoftwareRenderer::valid_rect(const Rect& rect) noexcept {
    return std::isfinite(rect.x) && std::isfinite(rect.y) &&
           std::isfinite(rect.width) && std::isfinite(rect.height) &&
           rect.width > 0.0f && rect.height > 0.0f;
}

std::unique_ptr<Renderer2D> create_software_renderer() {
    return std::make_unique<SoftwareRenderer>();
}

} // namespace storm
