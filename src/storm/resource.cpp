#include "storm/resource.hpp"

#include <fstream>
#include <iterator>
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
    const std::string& path) const {
    if (path.empty()) return nullptr;

    for (const auto& entry : resources_) {
        if (entry.path == path) return entry.resource;
    }

    return nullptr;
}

FileResourceProvider::FileResourceProvider(std::filesystem::path root)
    : root_(std::move(root)) {}

std::shared_ptr<const Resource> FileResourceProvider::load(
    const std::string& path) const {
    if (path.empty()) return nullptr;

    const std::filesystem::path relative(path);
    if (relative.is_absolute()) return nullptr;

    const auto candidate = root_ / relative;
    std::error_code ec;
    const auto canonical_root = std::filesystem::weakly_canonical(root_, ec);
    if (ec) return nullptr;
    const auto canonical_candidate = std::filesystem::weakly_canonical(candidate, ec);
    if (ec) return nullptr;

    auto root_it = canonical_root.begin();
    auto candidate_it = canonical_candidate.begin();
    for (; root_it != canonical_root.end(); ++root_it, ++candidate_it) {
        if (candidate_it == canonical_candidate.end() || *root_it != *candidate_it) {
            return nullptr;
        }
    }

    std::ifstream file(canonical_candidate, std::ios::binary);
    if (!file) return nullptr;

    std::vector<std::uint8_t> bytes{
        std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    return std::make_shared<const Resource>(std::move(bytes));
}

} // namespace storm
