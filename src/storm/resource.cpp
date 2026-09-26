#include "storm/resource.hpp"

#include <utility>

namespace storm {

Resource::Resource(std::vector<std::uint8_t> data)
    : data_(std::move(data)) {}

const std::vector<std::uint8_t>& Resource::data() const noexcept {
    return data_;
}

std::size_t Resource::size() const noexcept {
    return data_.size();
}

bool Resource::empty() const noexcept {
    return data_.empty();
}

void MemoryResourceProvider::put(std::string path,
                                 std::vector<std::uint8_t> data) {
    if (path.empty()) return;

    const auto resource = std::make_shared<const Resource>(std::move(data));
    for (auto& entry : resources_) {
        if (entry.path == path) {
            entry.resource = resource;
            return;
        }
    }

    resources_.push_back({std::move(path), resource});
}

void MemoryResourceProvider::remove(const std::string& path) {
    for (auto it = resources_.begin(); it != resources_.end(); ++it) {
        if (it->path == path) {
            resources_.erase(it);
            return;
        }
    }
}

void MemoryResourceProvider::clear() noexcept {
    resources_.clear();
}

std::shared_ptr<const Resource> MemoryResourceProvider::load(
    const std::string& path) {
    if (path.empty()) return nullptr;

    for (const auto& entry : resources_) {
        if (entry.path == path) return entry.resource;
    }

    return nullptr;
}

} // namespace storm
