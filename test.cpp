#include <unordered_map>
#include <vector>
#include <shared_mutex>
#include <mutex>
#include <optional>
#include <functional>
#include <iostream>
#include <string>

using namespace std;

template <typename Key, typename Value>
class ConcurrentMap {
public:
    ConcurrentMap(size_t numShards = 8) : shards(numShards) {}

    // Insert or update a value
    void insert(const Key& key, const Value& value) {
        auto& shard = getShard(key); // This will call the non-const version
        std::unique_lock<std::shared_mutex> lock(shard.mutex);  // Lock only this shard
        shard.map[key] = value;
    }

    // Find a value (returns std::optional)
    std::optional<Value> find(const Key& key) const {
        const auto& shard = getShard(key); // Call const version
        std::shared_lock<std::shared_mutex> lock(shard.mutex);  // Shared lock for read-only access
        auto it = shard.map.find(key);
        if (it != shard.map.end()) {
            return it->second;  // Return the found value
        }
        return std::nullopt;  // Return std::nullopt if not found
    }

    // Erase a key
    void erase(const Key& key) {
        auto& shard = getShard(key); // This will call the non-const version
        std::unique_lock<std::shared_mutex> lock(shard.mutex);  // Lock only this shard
        shard.map.erase(key);
    }

private:
    struct Shard {
        std::unordered_map<Key, Value> map;
        mutable std::shared_mutex mutex;  // Mutex for each shard
    };

    std::vector<Shard> shards;

    // Non-const version for non-const member functions
    Shard& getShard(const Key& key) {
        size_t hashValue = std::hash<Key>{}(key);
        return shards[hashValue % shards.size()];
    }

    // Const version for const member functions
    const Shard& getShard(const Key& key) const {
        size_t hashValue = std::hash<Key>{}(key);
        return shards[hashValue % shards.size()];
    }
};

int main() {
    ConcurrentMap<std::string, int> cmap(10);

    cmap.insert("key1", 42);
    
    auto value = cmap.find("key1");
    if (value) {
        std::cout << "Found: " << *value << std::endl;  // Dereference the optional
    } else {
        std::cout << "Not found" << std::endl;
    }

    value = cmap.find("key2");
    if (value) {
        std::cout << "Found: " << *value << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}
