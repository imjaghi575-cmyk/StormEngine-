#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>

namespace storm {

enum class KeyCode : std::uint16_t {
    Unknown = 0,
    Escape,
    Space,
    Enter,
    Left,
    Right,
    Up,
    Down,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z
};

enum class InputEventType : std::uint8_t {
    KeyPressed,
    KeyReleased
};

struct InputEvent {
    InputEventType type{InputEventType::KeyReleased};
    KeyCode key{KeyCode::Unknown};
};

class InputState {
public:
    void apply(const InputEvent& event) noexcept;
    bool is_down(KeyCode key) const noexcept;
    void clear() noexcept;

private:
    static constexpr std::size_t key_count =
        static_cast<std::size_t>(KeyCode::Z) + 1U;

    bool keys_[key_count]{};
};

class InputQueue {
public:
    void push(InputEvent event);
    bool empty() const noexcept;
    std::size_t size() const noexcept;
    InputEvent pop();
    void clear() noexcept;

private:
    std::deque<InputEvent> events_;
};

} // namespace storm
