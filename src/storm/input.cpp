#include "storm/input.hpp"

namespace storm {

void InputState::apply(const InputEvent& event) noexcept {
    switch (event.type) {
    case InputEventType::KeyPressed:
    case InputEventType::KeyReleased: {
        const auto index = static_cast<std::size_t>(event.key);
        if (index < key_count) {
            keys_[index] = event.type == InputEventType::KeyPressed;
        }
        break;
    }
    case InputEventType::PointerMoved:
        pointer_x_ = event.x;
        pointer_y_ = event.y;
        break;
    case InputEventType::PointerPressed:
    case InputEventType::PointerReleased: {
        const auto index = static_cast<std::size_t>(event.button);
        if (index < pointer_button_count) {
            pointer_buttons_[index] =
                event.type == InputEventType::PointerPressed;
        }
        pointer_x_ = event.x;
        pointer_y_ = event.y;
        break;
    }
    }
}

bool InputState::is_down(KeyCode key) const noexcept {
    if (key == KeyCode::Unknown) return false;
    const auto index = static_cast<std::size_t>(key);
    return index < key_count && keys_[index];
}

bool InputState::is_down(PointerButton button) const noexcept {
    if (button == PointerButton::None) return false;
    const auto index = static_cast<std::size_t>(button);
    return index < pointer_button_count && pointer_buttons_[index];
}

float InputState::pointer_x() const noexcept {
    return pointer_x_;
}

float InputState::pointer_y() const noexcept {
    return pointer_y_;
}

void InputState::clear() noexcept {
    for (bool& key : keys_) {
        key = false;
    }
    for (bool& button : pointer_buttons_) {
        button = false;
    }
    pointer_x_ = 0.0f;
    pointer_y_ = 0.0f;
}

void InputQueue::push(InputEvent event) {
    events_.push_back(event);
}

bool InputQueue::empty() const noexcept {
    return events_.empty();
}

std::size_t InputQueue::size() const noexcept {
    return events_.size();
}

bool InputQueue::try_pop(InputEvent& event) noexcept {
    if (events_.empty()) {
        return false;
    }

    event = events_.front();
    events_.pop_front();
    return true;
}

void InputQueue::clear() noexcept {
    events_.clear();
}

} // namespace storm
