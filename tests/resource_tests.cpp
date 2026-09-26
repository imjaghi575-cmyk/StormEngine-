#include "storm/resource.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>

int main() {
    storm::Resource empty;
    assert(empty.empty());
    assert(empty.size() == 0);

    storm::MemoryResourceProvider provider;
    assert(provider.load("missing") == nullptr);

    provider.put("textures/test.bin", {1, 2, 3, 4});
    provider.put("textures/./test.bin", {5, 6});
    auto normalized = provider.load("textures/test.bin");
    assert(normalized != nullptr);
    assert(normalized->data()[0] == 5);
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
    assert(provider.load("../escape") == nullptr);
    assert(provider.load("/absolute") == nullptr);
    provider.put("../escape", {7});
    assert(provider.load("../escape") == nullptr);

    provider.remove("textures/test.bin");
    assert(provider.load("textures/test.bin") == nullptr);

    provider.put("a", {42});
    provider.put("b", {24});
    provider.clear();
    assert(provider.load("a") == nullptr);
    assert(provider.load("b") == nullptr);

    const auto root = std::filesystem::temp_directory_path() /
                      "storm_engine_resource_tests";
    std::error_code ec;
    std::filesystem::remove_all(root, ec);
    assert(!ec);
    std::filesystem::create_directories(root / "assets", ec);
    assert(!ec);

    {
        std::ofstream file(root / "assets" / "data.bin", std::ios::binary);
        file.write("\x01\x02\x03", 3);
        assert(file.good());
    }

    const storm::FileResourceProvider files(root);
    auto disk = files.load("assets/data.bin");
    assert(disk != nullptr);
    assert(disk->size() == 3);
    assert(disk->data()[0] == 1);
    assert(disk->data()[2] == 3);

    assert(files.load("") == nullptr);
    assert(files.load("../data.bin") == nullptr);
    assert(files.load(root.string()) == nullptr);
    assert(files.load("missing.bin") == nullptr);

    std::filesystem::create_directories(root / "outside", ec);
    assert(!ec);
    std::filesystem::create_directory_symlink(
        root / "assets", root / "outside" / "link", ec);
    if (!ec) {
        assert(files.load("outside/link/data.bin") != nullptr);
    }

    std::filesystem::remove_all(root, ec);
    assert(!ec);

    return 0;
}
