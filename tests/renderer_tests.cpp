#include "storm/render.hpp"

#include <cassert>

int main() {
    auto renderer = storm::create_software_renderer();

    assert(renderer);
    assert(!renderer->initialize(0, 480, "invalid"));
    assert(renderer->initialize(640, 480, "Storm Renderer Test"));
    assert(renderer->width() == 640);
    assert(renderer->height() == 480);

    renderer->begin_frame({0, 0, 0, 255});
    renderer->draw_rect({10, 10, 32, 32}, {255, 255, 255, 255});
    renderer->draw_sprite({20, 20, 16, 16}, "test.png");
    renderer->end_frame();
    renderer->shutdown();

    assert(renderer->width() == 0);
    assert(renderer->height() == 0);
    return 0;
}
