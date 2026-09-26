#pragma once

#include <cstdint>
#include <memory>

namespace storm {

enum class Key : std::uint16_t {
    Unknown = 0,
    Escape,
    Enter,
    Space,
    Left,
    Right,
    Up,
    Down
};

struct InputState {
    bool quit_requested{false};

    bool key_down(Key key) const noexcept {
        return key == Key::Unknown ? false : keys[static_cast<std::size_t>(key)];
    }

    void set_key(Key key, bool down) noexcept {
        if (key != Key::Unknown) keys[static_cast<std::size_t>(key)] = down;
    }

private:
    bool keys[16]{};
};

class Platform {
public:
    virtual ~Platform() = default;

    virtual bool initialize(int width, int height, const char* title) = 0;
    virtual bool poll_events(InputState& input) = 0;
    virtual void shutdown() = 0;

    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
};

std::unique_ptr<Platform> create_headless_platform();

} // namespace storm
