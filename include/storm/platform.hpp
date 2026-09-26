#pragma once

#include <array>
#include <cstddef>
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
    Down,
    Count
};

struct InputState {
    bool quit_requested{false};

    bool key_down(Key key) const noexcept {
        const auto index = static_cast<std::size_t>(key);
        return index < keys.size() && keys[index];
    }

    void set_key(Key key, bool down) noexcept {
        const auto index = static_cast<std::size_t>(key);
        if (index < keys.size()) keys[index] = down;
    }

private:
    std::array<bool, static_cast<std::size_t>(Key::Count)> keys{};
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
