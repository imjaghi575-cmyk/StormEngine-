#include "storm/input.hpp"

#include <cassert>

int main() {
    storm::InputState state;

    assert(!state.is_down(storm::KeyCode::A));
    state.apply({storm::InputEventType::KeyPressed, storm::KeyCode::A});
    assert(state.is_down(storm::KeyCode::A));

    state.apply({storm::InputEventType::KeyReleased, storm::KeyCode::A});
    assert(!state.is_down(storm::KeyCode::A));

    state.apply({storm::InputEventType::KeyPressed, storm::KeyCode::Z});
    assert(state.is_down(storm::KeyCode::Z));
    state.clear();
    assert(!state.is_down(storm::KeyCode::Z));

    storm::InputQueue queue;
    assert(queue.empty());
    queue.push({storm::InputEventType::KeyPressed, storm::KeyCode::Left});
    queue.push({storm::InputEventType::KeyReleased, storm::KeyCode::Left});

    assert(queue.size() == 2);
    assert(queue.pop().type == storm::InputEventType::KeyPressed);
    assert(queue.pop().type == storm::InputEventType::KeyReleased);
    assert(queue.empty());

    queue.push({storm::InputEventType::KeyPressed, storm::KeyCode::Enter});
    queue.clear();
    assert(queue.empty());

    return 0;
}
