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
    const auto clear = software->framebuffer().front();
    assert(clear.r == 0 && clear.g == 0 && clear.b == 0 && clear.a == 255);
    renderer->draw_sprite({20, 20, 16, 16}, "");
    renderer->end_frame();

    assert(renderer->initialize(640, 480, "Storm Renderer Test"));
    assert(renderer->width() == 640);
    assert(renderer->height() == 480);

    auto* software = dynamic_cast<storm::SoftwareRenderer*>(renderer.get());
    assert(software != nullptr);
    assert(software->framebuffer().size() == 640U * 480U);

    renderer->begin_frame({0, 0, 0, 255});
    renderer->draw_rect({10, 10, 32, 32}, {255, 255, 255, 255});
    renderer->draw_rect({
        std::numeric_limits<float>::quiet_NaN(), 10, 32, 32
    }, {255, 255, 255, 255});
    renderer->draw_sprite({20, 20, 16, 16}, "test.png");
    assert(software->framebuffer()[10U * 640U + 10U].r == 255);
    assert(software->framebuffer()[10U * 640U + 10U].g == 255);
    assert(software->framebuffer()[20U * 640U + 20U].b == 255);
    renderer->draw_sprite({20, 20, 16, 16}, "");
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
