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

    assert(!input.key_down(storm::Key::Space));

    input.set_key(storm::Key::Space, true);
    assert(input.key_down(storm::Key::Space));

    assert(platform->poll_events(input));
    platform->shutdown();

    assert(platform->width() == 0);
    assert(platform->height() == 0);
    assert(!platform->poll_events(input));

    return 0;
}
