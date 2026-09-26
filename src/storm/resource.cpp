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

FileResourceProvider::FileResourceProvider(std::filesystem::path root)
    : root_(std::move(root)) {}

std::shared_ptr<const Resource> FileResourceProvider::load(
    const std::string& path) const {
    if (path.empty()) return nullptr;

    const std::filesystem::path relative(path);
    if (relative.is_absolute()) return nullptr;

    const auto full_path = root_ / relative;

    std::error_code error;
    const auto root_canonical = std::filesystem::weakly_canonical(root_, error);
    if (error) return nullptr;

    error.clear();
    const auto file_canonical =
        std::filesystem::weakly_canonical(full_path, error);
    if (error || file_canonical == root_canonical) return nullptr;

    auto relative_to_root =
        std::filesystem::relative(file_canonical, root_canonical, error);
    if (error || relative_to_root.empty() ||
        relative_to_root == std::filesystem::path(".") ||
        *relative_to_root.begin() == std::filesystem::path("..")) {
        return nullptr;
    }

    std::ifstream file(file_canonical, std::ios::binary);
    if (!file) return nullptr;

    file.seekg(0, std::ios::end);
    const auto end = file.tellg();
    if (end < 0) return nullptr;

    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(end));
    file.seekg(0, std::ios::beg);

    if (!bytes.empty()) {
        file.read(reinterpret_cast<char*>(bytes.data()),
                  static_cast<std::streamsize>(bytes.size()));
        if (!file) return nullptr;
    }

    return std::make_shared<const Resource>(std::move(bytes));
}

void MemoryResourceProvider::put(std::string path,
                                 std::vector<std::uint8_t> data) {
    if (path.empty()) return;

    resources_[std::move(path)] =
        std::make_shared<const Resource>(std::move(data));
}

void MemoryResourceProvider::remove(const std::string& path) {
    if (path.empty()) return;
    resources_.erase(path);
}

void MemoryResourceProvider::clear() noexcept {
    resources_.clear();
}

std::shared_ptr<const Resource> MemoryResourceProvider::load(
    const std::string& path) const {
    if (path.empty()) return nullptr;

    const auto it = resources_.find(path);
    return it == resources_.end() ? nullptr : it->second;
}

} // namespace storm
