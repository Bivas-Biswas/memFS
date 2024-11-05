#ifndef CONCURRENTMAP_H
#define CONCURRENTMAP_H

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
class ConcurrentMap
{
public:
    ConcurrentMap(size_t numShards = 8) : shards(numShards) {}

    // Insert or update a value
    void insert(const Key &key, const Value &value)
    {
        auto &shard = getShard(key);                           // This will call the non-const version
        std::unique_lock<std::shared_mutex> lock(shard.mutex); // Lock only this shard
        shard.map[key] = value;
    }

    // Find a value (returns std::optional)
    Value find(const Key &key) const
    {
        const auto &shard = getShard(key);                     // Call const version
        std::shared_lock<std::shared_mutex> lock(shard.mutex); // Shared lock for read-only access
        auto it = shard.map.find(key);
        if (it != shard.map.end())
        {
            return it->second; // Return the found value
        }
        return nullptr; // Return std::nullopt if not found
    }

    // Erase a key
    Value erase(const Key &key)
    {
        auto &shard = getShard(key);                           // This will call the non-const version
        std::unique_lock<std::shared_mutex> lock(shard.mutex); // Lock only this shard
        auto it = shard.map.find(key);
        if (it != shard.map.end())
        {
            auto value = it->second;
            shard.map.erase(key);
            return value; // Return the found value
        }
        return nullptr;
    }

    void traverse(const std::function<void(const Key &, const Value &)> &callback) const
    {
        for (const auto &shard : shards)
        {
            // std::shared_lock<std::shared_mutex> lock(shard.mutex); // Shared lock for read-only access
            for (const auto &pair : shard.map)
            {
                callback(pair.first, pair.second); // Call the callback function
            }
        }
    }

    void empty()
    {
        for (auto &shard : shards)
        {
            // std::unique_lock<std::shared_mutex> lock(shard.mutex);  // Lock for write access
            shard.map.clear(); // Clear the unordered_map in the shard
        }
    }

private:
    struct Shard
    {
        std::unordered_map<Key, Value> map;
        mutable std::shared_mutex mutex; // Mutex for each shard
    };

    std::vector<Shard> shards;

    // Non-const version for non-const member functions
    Shard &getShard(const Key &key)
    {
        size_t hashValue = std::hash<Key>{}(key);
        return shards[hashValue % shards.size()];
    }

    // Const version for const member functions
    const Shard &getShard(const Key &key) const
    {
        size_t hashValue = std::hash<Key>{}(key);
        return shards[hashValue % shards.size()];
    }
};

#endif