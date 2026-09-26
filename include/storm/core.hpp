#pragma once

#include "storm/input.hpp"
#include "storm/time.hpp"

#include <cmath>
#include <memory>
#include <string>
#include <vector>

namespace storm {

struct Vec2 {
    float x{0.0f};
    float y{0.0f};

    constexpr Vec2() = default;
    constexpr Vec2(float x_value, float y_value) : x(x_value), y(y_value) {}

    constexpr Vec2 operator+(const Vec2& other) const {
        return {x + other.x, y + other.y};
    }

    constexpr Vec2 operator-(const Vec2& other) const {
        return {x - other.x, y - other.y};
    }

    constexpr Vec2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    float length() const noexcept {
        return std::sqrt(x * x + y * y);
    }
};

struct Transform2D {
    Vec2 position{};
    Vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};
};

class Node {
public:
    explicit Node(std::string name = "Node");
    virtual ~Node() = default;

    const std::string& name() const;
    void set_name(std::string name);

    const Transform2D& transform() const;
    Transform2D& transform();

    Vec2 position() const;
    void set_position(Vec2 position);

    Node* parent() const noexcept;
    void add_child(std::unique_ptr<Node> child);
    const std::vector<std::unique_ptr<Node>>& children() const noexcept;

    virtual void update(double delta_seconds);

protected:
    Node* parent_{nullptr};

private:
    std::string name_;
    Transform2D transform_{};
    std::vector<std::unique_ptr<Node>> children_;
};

class Scene {
public:
    Scene();

    Node& root() noexcept;
    const Node& root() const noexcept;

    void update(double delta_seconds);

private:
    Node root_;
};

class Engine {
public:
    struct Config {
        std::string name{"Storm Engine"};
        double max_delta_seconds{0.1};
    };

    Engine();
    explicit Engine(Config config);

    void set_scene(std::unique_ptr<Scene> scene);
    Scene* scene() noexcept;

    void tick(double delta_seconds);
    void process_input();
    InputState& input() noexcept;
    const InputState& input() const noexcept;
    void run_for(double seconds, double fixed_step = 1.0 / 60.0);

    const Config& config() const noexcept;

private:
    Config config_;
    FrameClock clock_;
    InputQueue input_queue_;
    InputState input_state_;
    std::unique_ptr<Scene> scene_;
};

} // namespace storm
