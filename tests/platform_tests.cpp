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

    input.apply_event({
        storm::InputEventType::KeyDown,
        storm::Key::Space
    });
    assert(input.key_down(storm::Key::Space));

    input.apply_event({
        storm::InputEventType::KeyUp,
        storm::Key::Space
    });
    assert(!input.key_down(storm::Key::Space));

    input.apply_event({
        storm::InputEventType::KeyDown,
        storm::Key::Escape
    });
    assert(input.key_down(storm::Key::Escape));

    input.apply_event({
        storm::InputEventType::Quit,
        storm::Key::Unknown
    });
    assert(input.quit_requested);

    input.set_key(storm::Key::Unknown, true);
    assert(!input.key_down(storm::Key::Unknown));

    input.set_key(storm::Key::Count, true);
    assert(!input.key_down(storm::Key::Count));

    const auto invalid_key = static_cast<storm::Key>(999);
    input.set_key(invalid_key, true);
    assert(!input.key_down(invalid_key));

    assert(platform->poll_events(input));
    platform->shutdown();

    assert(platform->width() == 0);
    assert(platform->height() == 0);
    assert(!platform->poll_events(input));

    return 0;
}
