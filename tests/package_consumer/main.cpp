#include "storm/core.hpp"

int main() {
    storm::Engine engine;
    return engine.config().name.empty() ? 1 : 0;
}
