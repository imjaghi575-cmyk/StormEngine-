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

enum class PointerButton : std::uint8_t {
    None = 0,
    Primary,
    Secondary,
    Middle
};

enum class InputEventType : std::uint8_t {
    KeyPressed,
    KeyReleased,
    Quit,
    PointerMoved,
    PointerPressed,
    PointerReleased
};

struct InputEvent {
    InputEventType type{InputEventType::KeyReleased};
    KeyCode key{KeyCode::Unknown};
    PointerButton button{PointerButton::None};
    float x{0.0f};
    float y{0.0f};
};

class InputState {
public:
    void apply(const InputEvent& event) noexcept;

    bool is_down(KeyCode key) const noexcept;
    bool is_down(PointerButton button) const noexcept;

    float pointer_x() const noexcept;
    float pointer_y() const noexcept;
    bool quit_requested() const noexcept;

    void clear() noexcept;

private:
    static constexpr std::size_t key_count =
        static_cast<std::size_t>(KeyCode::Z) + 1U;
    static constexpr std::size_t pointer_button_count =
        static_cast<std::size_t>(PointerButton::Middle) + 1U;

    bool keys_[key_count]{};
    bool pointer_buttons_[pointer_button_count]{};
    float pointer_x_{0.0f};
    float pointer_y_{0.0f};
    bool quit_requested_{false};
};

class InputQueue {
public:
    void push(InputEvent event);
    bool empty() const noexcept;
    std::size_t size() const noexcept;
    bool try_pop(InputEvent& event) noexcept;
    void clear() noexcept;

private:
    std::deque<InputEvent> events_;
};

} // namespace storm
