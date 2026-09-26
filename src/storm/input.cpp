#include "storm/input.hpp"


namespace storm {

void InputState::apply(const InputEvent& event) noexcept {
    const auto index = static_cast<std::size_t>(event.key);
    if (index >= key_count) {
        return;
    }

    keys_[index] = event.type == InputEventType::KeyPressed;
}

bool InputState::is_down(KeyCode key) const noexcept {
    const auto index = static_cast<std::size_t>(key);
    return index < key_count && keys_[index];
}

void InputState::clear() noexcept {
    for (bool& key : keys_) {
        key = false;
    }
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
