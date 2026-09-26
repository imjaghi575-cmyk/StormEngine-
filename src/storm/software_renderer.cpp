#include "storm/software_renderer.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <utility>

namespace storm {

namespace {

Color blend_source_over(Color source, Color destination) noexcept {
    if (source.a == 0) return destination;
    if (source.a == 255) return source;

    const std::uint64_t sa = source.a;
    const std::uint64_t da = destination.a;
    const std::uint64_t inverse_sa = 255U - sa;
    const std::uint64_t out_a = sa + (da * inverse_sa + 127U) / 255U;
    if (out_a == 0) return Color{0, 0, 0, 0};

    const auto blend_channel = [sa, da, inverse_sa, out_a](
                                   std::uint8_t source_channel,
                                   std::uint8_t destination_channel) {
        const std::uint64_t numerator =
            static_cast<std::uint64_t>(source_channel) * sa * 255U +
            static_cast<std::uint64_t>(destination_channel) * da *
                inverse_sa;
        const std::uint64_t denominator = 255U * out_a;
        return static_cast<std::uint8_t>(
            (numerator + denominator / 2U) / denominator);
    };

    return {
        blend_channel(source.r, destination.r),
        blend_channel(source.g, destination.g),
        blend_channel(source.b, destination.b),
        static_cast<std::uint8_t>(out_a)
    };
}

} // namespace

Texture2D::Texture2D(int width, int height, std::vector<Color> pixels)
    : width_(width), height_(height), pixels_(std::move(pixels)) {
    if (!valid()) {
        width_ = 0;
        height_ = 0;
        pixels_.clear();
    }
}

bool Texture2D::valid() const noexcept {
    if (width_ <= 0 || height_ <= 0) return false;
    const auto w = static_cast<std::size_t>(width_);
    const auto h = static_cast<std::size_t>(height_);
    return h <= std::numeric_limits<std::size_t>::max() / w &&
           pixels_.size() == w * h;
}

bool SoftwareRenderer::initialize(int width, int height,
                                  const std::string& title) {
    if (width <= 0 || height <= 0) {
        shutdown();
        return false;
    }

    const auto w = static_cast<std::size_t>(width);
    const auto h = static_cast<std::size_t>(height);
    if (h > std::numeric_limits<std::size_t>::max() / w) {
        shutdown();
        return false;
    }
    const auto pixel_count = w * h;
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

    const double x0 = static_cast<double>(rect.x);
    const double y0 = static_cast<double>(rect.y);
    const double x1 = x0 + static_cast<double>(rect.width);
    const double y1 = y0 + static_cast<double>(rect.height);

    if (x1 <= 0.0 || y1 <= 0.0 ||
        x0 >= static_cast<double>(width_) ||
        y0 >= static_cast<double>(height_)) {
        return;
    }

    const int left = x0 <= 0.0 ? 0 : static_cast<int>(std::floor(x0));
    const int top = y0 <= 0.0 ? 0 : static_cast<int>(std::floor(y0));
    const int right = x1 >= static_cast<double>(width_)
                          ? width_
                          : static_cast<int>(std::ceil(x1));
    const int bottom = y1 >= static_cast<double>(height_)
                           ? height_
                           : static_cast<int>(std::ceil(y1));

    for (int y = top; y < bottom; ++y) {
        for (int x = left; x < right; ++x) {
            auto& destination =
                pixels_[static_cast<std::size_t>(y) *
                            static_cast<std::size_t>(width_) +
                        static_cast<std::size_t>(x)];
            destination = blend_source_over(color, destination);
        }
    }
}

void SoftwareRenderer::draw_sprite(const Rect& destination,
                                    const Texture2D& texture) {
    if (!initialized_ || !valid_rect(destination) || !texture.valid()) return;

    const double x0 = static_cast<double>(destination.x);
    const double y0 = static_cast<double>(destination.y);
    const double x1 = x0 + static_cast<double>(destination.width);
    const double y1 = y0 + static_cast<double>(destination.height);

    if (x1 <= 0.0 || y1 <= 0.0 ||
        x0 >= static_cast<double>(width_) ||
        y0 >= static_cast<double>(height_)) {
        return;
    }

    const int left = x0 <= 0.0 ? 0 : static_cast<int>(std::floor(x0));
    const int top = y0 <= 0.0 ? 0 : static_cast<int>(std::floor(y0));
    const double clipped_x1 = std::min(x1, static_cast<double>(width_));
    const double clipped_y1 = std::min(y1, static_cast<double>(height_));
    const int right = std::min(width_, static_cast<int>(std::ceil(clipped_x1)));
    const int bottom = std::min(height_, static_cast<int>(std::ceil(clipped_y1)));

    const auto& source = texture.pixels();
    const int source_width = texture.width();
    const int source_height = texture.height();

    for (int y = top; y < bottom; ++y) {
        const double v = (static_cast<double>(y) + 0.5 - y0) /
                         static_cast<double>(destination.height);
        const int sy = std::clamp(
            static_cast<int>(v * static_cast<double>(source_height)),
            0, source_height - 1);
        for (int x = left; x < right; ++x) {
            const double u = (static_cast<double>(x) + 0.5 - x0) /
                             static_cast<double>(destination.width);
            const int sx = std::clamp(
                static_cast<int>(u * static_cast<double>(source_width)),
                0, source_width - 1);
            auto& destination_pixel =
                pixels_[static_cast<std::size_t>(y) *
                            static_cast<std::size_t>(width_) +
                        static_cast<std::size_t>(x)];
            const auto source_pixel =
                source[static_cast<std::size_t>(sy) *
                           static_cast<std::size_t>(source_width) +
                       static_cast<std::size_t>(sx)];
            destination_pixel =
                blend_source_over(source_pixel, destination_pixel);
        }
    }
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
