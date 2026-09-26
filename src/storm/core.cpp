#include "storm/core.hpp"

#include <algorithm>
#include <chrono>
#include <thread>
#include <utility>

namespace storm {

Node::Node(std::string name)
    : name_(std::move(name)) {}

const std::string& Node::name() const { return name_; }

void Node::set_name(std::string name) { name_ = std::move(name); }

const Transform2D& Node::transform() const { return transform_; }

Transform2D& Node::transform() { return transform_; }

Vec2 Node::position() const { return transform_.position; }

void Node::set_position(Vec2 position) { transform_.position = position; }

Node* Node::parent() const noexcept { return parent_; }

void Node::add_child(std::unique_ptr<Node> child) {
    if (!child) return;
    child->parent_ = this;
    children_.push_back(std::move(child));
}

const std::vector<std::unique_ptr<Node>>& Node::children() const noexcept {
    return children_;
}

void Node::update(double delta_seconds) {
    for (const auto& child : children_) {
        child->update(delta_seconds);
    }
}

Scene::Scene()
    : root_("Root") {}

Node& Scene::root() noexcept { return root_; }

const Node& Scene::root() const noexcept { return root_; }

void Scene::update(double delta_seconds) {
    root_.update(delta_seconds);
}

Engine::Engine(Config config)
    : config_(std::move(config)) {}

void Engine::set_scene(std::unique_ptr<Scene> scene) {
    scene_ = std::move(scene);
}

Scene* Engine::scene() noexcept {
    return scene_.get();
}

void Engine::tick(double delta_seconds) {
    if (!scene_) return;

    const double clamped_delta =
        std::clamp(delta_seconds, 0.0, config_.max_delta_seconds);

    scene_->update(clamped_delta);
}

void Engine::run_for(double seconds, double fixed_step) {
    if (seconds <= 0.0 || fixed_step <= 0.0) return;

    const auto start = std::chrono::steady_clock::now();
    auto previous = start;

    while (true) {
        const auto now = std::chrono::steady_clock::now();
        const double elapsed =
            std::chrono::duration<double>(now - start).count();

        if (elapsed >= seconds) break;

        const double frame_delta =
            std::chrono::duration<double>(now - previous).count();
        previous = now;

        tick(frame_delta);

        const auto sleep_time = std::chrono::duration<double>(fixed_step);
        const auto after_tick = std::chrono::steady_clock::now();
        const auto spent = after_tick - now;

        if (spent < sleep_time) {
            std::this_thread::sleep_for(sleep_time - spent);
        }
    }
}

const Engine::Config& Engine::config() const noexcept {
    return config_;
}

} // namespace storm
