#include "storm/core.hpp"

#include <iostream>
#include <memory>

class PlayerNode final : public storm::Node {
public:
    PlayerNode()
        : storm::Node("Player") {}

    void update(double delta_seconds) override {
        auto p = position();
        p.x += static_cast<float>(delta_seconds);
        set_position(p);

        storm::Node::update(delta_seconds);
    }
};

int main() {
    storm::Engine engine;

    auto scene = std::make_unique<storm::Scene>();
    scene->root().add_child(std::make_unique<PlayerNode>());
    engine.set_scene(std::move(scene));

    engine.run_for(0.2);

    const auto& player = *engine.scene()->root().children().front();
    std::cout << engine.config().name << "\n";
    std::cout << "Player position: "
              << player.position().x << ", "
              << player.position().y << "\n";

    return 0;
}
