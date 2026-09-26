#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
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
        const std::string& path) = 0;
};

class MemoryResourceProvider final : public ResourceProvider {
public:
    void put(std::string path, std::vector<std::uint8_t> data);
    void remove(const std::string& path);
    void clear() noexcept;

    std::shared_ptr<const Resource> load(
        const std::string& path) override;

private:
    struct Entry {
        std::string path;
        std::shared_ptr<const Resource> resource;
    };

    std::vector<Entry> resources_;
};

} // namespace storm
