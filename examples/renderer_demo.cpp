#include "storm/render.hpp"

int main() {
    auto renderer = storm::create_software_renderer();

    if (!renderer->initialize(800, 450, "Storm Engine Demo")) return 1;

    renderer->begin_frame({20, 30, 45, 255});
    renderer->draw_rect({100, 100, 200, 120}, {40, 120, 220, 255});
    renderer->draw_sprite({350, 150, 128, 128}, "player.png");
    renderer->end_frame();
    renderer->shutdown();

    return 0;
}
