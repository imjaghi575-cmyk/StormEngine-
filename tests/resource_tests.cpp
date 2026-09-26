#include "storm/resource.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

int main() {
    storm::Resource empty;
    assert(empty.empty());
    assert(empty.size() == 0);

    storm::MemoryResourceProvider provider;
    assert(provider.load("missing") == nullptr);

    provider.put("textures/test.bin", {1, 2, 3, 4});
    auto first = provider.load("textures/test.bin");
    assert(first != nullptr);
    assert(first->size() == 4);
    assert(first->data()[0] == 1);
    assert(first->data()[3] == 4);

    provider.put("textures/test.bin", {9, 8});
    auto replaced = provider.load("textures/test.bin");
    assert(replaced != nullptr);
    assert(replaced->size() == 2);
    assert(replaced->data()[0] == 9);
    assert(first->size() == 4);

    provider.put("", {7});
    assert(provider.load("") == nullptr);

    provider.remove("textures/test.bin");
    assert(provider.load("textures/test.bin") == nullptr);

    provider.put("a", {42});
    provider.put("b", {24});
    provider.clear();
    assert(provider.load("a") == nullptr);
    assert(provider.load("b") == nullptr);

    return 0;
}
