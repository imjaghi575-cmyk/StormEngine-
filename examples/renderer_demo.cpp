#include "storm/render.hpp"

int main() {
    auto renderer = storm::create_software_renderer();
    if (!renderer->initialize(640, 360, "Storm Renderer Demo")) return 1;

    renderer->begin_frame({20, 24, 32, 255});
    renderer->draw_rect({40, 40, 200, 100}, {40, 120, 220, 255});

    const storm::Texture2D texture(2, 2, {
        {255, 80, 80, 255}, {80, 255, 80, 255},
        {80, 80, 255, 255}, {255, 255, 255, 255}
    });
    renderer->draw_sprite({350, 150, 128, 128}, texture);

    renderer->end_frame();
    renderer->shutdown();
    return 0;
}
