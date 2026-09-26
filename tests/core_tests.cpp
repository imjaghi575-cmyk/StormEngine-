#include "storm/core.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <memory>

class TestNode final : public storm::Node {
public:
    TestNode() : storm::Node("Test") {}

    void update(double delta_seconds) override {
        auto p = position();
        p.x += static_cast<float>(delta_seconds);
        set_position(p);
        storm::Node::update(delta_seconds);
    }
};

int main() {
    storm::Vec2 a{3.0f, 4.0f};
    assert(std::abs(a.length() - 5.0f) < 0.0001f);
    assert((a + storm::Vec2{1.0f, 2.0f}).x == 4.0f);
    assert((a - storm::Vec2{1.0f, 2.0f}).y == 2.0f);

    storm::Scene scene;
    auto child = std::make_unique<TestNode>();
    auto* child_ptr = child.get();
    scene.root().add_child(std::move(child));

    scene.update(0.5);
    assert(std::abs(child_ptr->position().x - 0.5f) < 0.0001f);

    child_ptr->transform().scale = {2.0f, 3.0f};
    child_ptr->transform().rotation = 1.0f;

    assert(child_ptr->transform().scale.x == 2.0f);
    assert(child_ptr->transform().scale.y == 3.0f);
    assert(child_ptr->transform().rotation == 1.0f);

    storm::Engine engine;
    assert(engine.scene() == nullptr);

    auto engine_scene = std::make_unique<storm::Scene>();
    auto* engine_root = &engine_scene->root();
    auto engine_child = std::make_unique<TestNode>();
    auto* engine_child_ptr = engine_child.get();
    engine_root->add_child(std::move(engine_child));

    engine.set_scene(std::move(engine_scene));
    assert(engine.scene() != nullptr);

    engine.tick(1.0);
    assert(std::abs(engine_child_ptr->position().x - 0.1f) < 0.0001f);

    engine.tick(-1.0);
    assert(std::abs(engine_child_ptr->position().x - 0.1f) < 0.0001f);

    engine.tick(std::numeric_limits<double>::quiet_NaN());
    assert(std::isfinite(engine_child_ptr->position().x));

    const float before_invalid_runs = engine_child_ptr->position().x;
    engine.run_for(-1.0);
    engine.run_for(0.0);
    engine.run_for(0.01, 0.0);
    engine.run_for(0.01, -1.0);
    engine.run_for(std::numeric_limits<double>::quiet_NaN());
    engine.run_for(0.01, std::numeric_limits<double>::quiet_NaN());
    assert(std::abs(engine_child_ptr->position().x - before_invalid_runs) < 0.0001f);

    storm::Engine configured_engine({
        "Configured",
        0.25
    });
    auto configured_scene = std::make_unique<storm::Scene>();
    auto configured_child = std::make_unique<TestNode>();
    auto* configured_child_ptr = configured_child.get();
    configured_scene->root().add_child(std::move(configured_child));
    configured_engine.set_scene(std::move(configured_scene));

    configured_engine.tick(1.0);
    assert(std::abs(configured_child_ptr->position().x - 0.25f) < 0.0001f);

    storm::Engine invalid_engine({
        "Invalid Config",
        -1.0
    });
    auto invalid_scene = std::make_unique<storm::Scene>();
    auto* invalid_root = &invalid_scene->root();
    auto invalid_child = std::make_unique<TestNode>();
    auto* invalid_child_ptr = invalid_child.get();
    invalid_root->add_child(std::move(invalid_child));
    invalid_engine.set_scene(std::move(invalid_scene));

    invalid_engine.tick(1.0);
    assert(std::abs(invalid_child_ptr->position().x) < 0.0001f);

    engine.set_scene(nullptr);
    assert(engine.scene() == nullptr);

    return 0;
}
