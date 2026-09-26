#include "storm/input.hpp"

#include <cassert>
#include <limits>

int main() {
    storm::InputState state;

    assert(!state.is_down(storm::KeyCode::A));
    assert(!state.quit_requested());
    state.apply({storm::InputEventType::KeyPressed, storm::KeyCode::A});
    assert(state.is_down(storm::KeyCode::A));

    state.apply({storm::InputEventType::KeyReleased, storm::KeyCode::A});
    assert(!state.is_down(storm::KeyCode::A));

    state.apply({storm::InputEventType::KeyPressed, storm::KeyCode::Z});
    assert(state.is_down(storm::KeyCode::Z));
    state.clear();
    assert(!state.is_down(storm::KeyCode::Z));
    state.apply({storm::InputEventType::Quit});
    assert(state.quit_requested());
    state.clear();
    assert(!state.quit_requested());

    storm::InputQueue queue;
    assert(queue.empty());
    queue.push({storm::InputEventType::KeyPressed, storm::KeyCode::Left});
    queue.push({storm::InputEventType::KeyReleased, storm::KeyCode::Left});

    assert(queue.size() == 2);
    storm::InputEvent event;
    assert(queue.try_pop(event));
    assert(event.type == storm::InputEventType::KeyPressed);
    assert(queue.try_pop(event));
    assert(event.type == storm::InputEventType::KeyReleased);
    assert(queue.empty());
    assert(!queue.try_pop(event));

    queue.push({storm::InputEventType::KeyPressed, storm::KeyCode::Enter});
    queue.clear();
    assert(queue.empty());

    state.apply({
        storm::InputEventType::PointerMoved,
        storm::KeyCode::Unknown,
        storm::PointerButton::None,
        12.5f,
        24.0f
    });
    assert(state.pointer_x() == 12.5f);
    assert(state.pointer_y() == 24.0f);
    assert(!state.is_down(storm::PointerButton::Primary));

    state.apply({
        storm::InputEventType::PointerPressed,
        storm::KeyCode::Unknown,
        storm::PointerButton::Primary,
        12.5f,
        24.0f
    });
    assert(state.is_down(storm::PointerButton::Primary));

    state.apply({
        storm::InputEventType::PointerReleased,
        storm::KeyCode::Unknown,
        storm::PointerButton::Primary,
        13.0f,
        25.0f
    });
    assert(!state.is_down(storm::PointerButton::Primary));
    assert(state.pointer_x() == 13.0f);
    assert(state.pointer_y() == 25.0f);

    state.apply({
        storm::InputEventType::PointerMoved,
        storm::KeyCode::Unknown,
        storm::PointerButton::None,
        std::numeric_limits<float>::quiet_NaN(),
        std::numeric_limits<float>::infinity()
    });
    assert(state.pointer_x() == 13.0f);
    assert(state.pointer_y() == 25.0f);

    state.clear();
    assert(!state.is_down(storm::PointerButton::Primary));
    assert(state.pointer_x() == 0.0f);
    assert(state.pointer_y() == 0.0f);

    return 0;
}
