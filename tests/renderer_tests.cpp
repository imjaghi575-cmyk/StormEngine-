#include "storm/render.hpp"
#include "storm/software_renderer.hpp"

#include <cassert>
#include <limits>

int main() {
    auto renderer = storm::create_software_renderer();

    assert(renderer);
    assert(!renderer->initialize(0, 480, "invalid"));
    assert(renderer->width() == 0);
    assert(renderer->height() == 0);

    renderer->begin_frame({0, 0, 0, 255});
    renderer->draw_rect({10, 10, 32, 32}, {255, 255, 255, 255});
    renderer->draw_sprite({20, 20, 16, 16}, storm::Texture2D{});
    renderer->end_frame();

    assert(renderer->initialize(640, 480, "Storm Renderer Test"));
    assert(renderer->width() == 640);
    assert(renderer->height() == 480);

    auto* software = dynamic_cast<storm::SoftwareRenderer*>(renderer.get());
    assert(software != nullptr);
    assert(software->framebuffer().size() == 640U * 480U);

    const auto clear = software->framebuffer().front();
    assert(clear.r == 255 && clear.g == 255 && clear.b == 255 && clear.a == 255);

    renderer->begin_frame({0, 0, 0, 255});
    renderer->draw_rect({10, 10, 32, 32}, {255, 255, 255, 255});
    renderer->draw_rect({-5, -5, 10, 10}, {10, 20, 30, 255});
    renderer->draw_rect({
        std::numeric_limits<float>::quiet_NaN(), 10, 32, 32
    }, {255, 255, 255, 255});
    renderer->draw_rect({
        std::numeric_limits<float>::max(), 10, 32, 32
    }, {255, 255, 255, 255});
    renderer->draw_rect({
        -std::numeric_limits<float>::max(), 10, 32, 32
    }, {255, 255, 255, 255});
    const storm::Texture2D texture(2, 2, {
        {255, 0, 0, 255}, {0, 255, 0, 255},
        {0, 0, 255, 255}, {255, 255, 255, 255}
    });
    assert(texture.valid());
    storm::Texture2D invalid_texture(2, 2, {{255, 255, 255, 255}});
    assert(!invalid_texture.valid());
    storm::Texture2D huge_texture(std::numeric_limits<int>::max(),
                                  std::numeric_limits<int>::max(), {});
    assert(!huge_texture.valid());
    renderer->draw_sprite({20, 20, 16, 16}, texture);
    assert(software->framebuffer()[20U * 640U + 20U].r == 255);
    assert(software->framebuffer()[20U * 640U + 28U].g == 255);
    assert(software->framebuffer()[28U * 640U + 20U].b == 255);
    assert(software->framebuffer()[10U * 640U + 10U].r == 255);
    assert(software->framebuffer().front().r == 10);
    assert(software->framebuffer().front().g == 20);
    assert(software->framebuffer()[10U * 640U + 10U].g == 255);
    assert(software->framebuffer()[20U * 640U + 20U].b == 255);
    renderer->draw_sprite({20, 20, 16, 16}, storm::Texture2D{});
    renderer->end_frame();

    renderer->shutdown();
    assert(software->framebuffer().empty());
    assert(renderer->width() == 0);
    assert(renderer->height() == 0);

    renderer->draw_rect({0, 0, 1, 1}, {255, 0, 0, 255});
    renderer->end_frame();

    assert(renderer->initialize(320, 240, "Reinitialize"));
    assert(renderer->width() == 320);
    assert(renderer->height() == 240);

    renderer->shutdown();
    renderer->shutdown();
    assert(renderer->width() == 0);
    assert(renderer->height() == 0);

    return 0;
}
