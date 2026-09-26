#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace storm {

class Resource {
public:
    Resource() = default;
    explicit Resource(std::vector<std::uint8_t> data);

    const std::vector<std::uint8_t>& data() const noexcept;
    std::size_t size() const noexcept;
    bool empty() const noexcept;

private:
    std::vector<std::uint8_t> data_;
};

class ResourceProvider {
public:
    virtual ~ResourceProvider() = default;

    virtual std::shared_ptr<const Resource> load(
        const std::string& path) const = 0;
};

class FileResourceProvider final : public ResourceProvider {
public:
    explicit FileResourceProvider(std::filesystem::path root);

    std::shared_ptr<const Resource> load(
        const std::string& path) const override;

private:
    std::filesystem::path root_;
};

class MemoryResourceProvider final : public ResourceProvider {
public:
    void put(std::string path, std::vector<std::uint8_t> data);
    void remove(const std::string& path);
    void clear() noexcept;

    std::shared_ptr<const Resource> load(
        const std::string& path) const override;

private:
    std::unordered_map<std::string, std::shared_ptr<const Resource>> resources_;
};

} // namespace storm
