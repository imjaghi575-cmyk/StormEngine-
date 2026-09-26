#include "storm/platform.hpp"

#include <cassert>

int main() {
    auto platform = storm::create_headless_platform();

    storm::InputState input;
    assert(!platform->poll_events(input));
    assert(!platform->initialize(0, 720, "invalid"));
    assert(platform->width() == 0);
    assert(platform->height() == 0);

    assert(platform->initialize(1280, 720, "Storm Test"));
    assert(platform->width() == 1280);
    assert(platform->height() == 720);

    assert(!input.is_down(storm::KeyCode::Space));

    input.apply({storm::InputEventType::KeyPressed, storm::KeyCode::Space});
    assert(input.is_down(storm::KeyCode::Space));

    input.apply({storm::InputEventType::KeyReleased, storm::KeyCode::Space});
    assert(!input.is_down(storm::KeyCode::Space));

    input.apply({storm::InputEventType::KeyPressed, storm::KeyCode::Escape});
    assert(input.is_down(storm::KeyCode::Escape));

    input.apply({storm::InputEventType::Quit});
    assert(input.quit_requested());

    input.apply({storm::InputEventType::KeyPressed,
                 static_cast<storm::KeyCode>(999)});
    assert(!input.is_down(static_cast<storm::KeyCode>(999)));

    platform->shutdown();

    assert(platform->width() == 0);
    assert(platform->height() == 0);
    assert(!platform->poll_events(input));

    return 0;
}
