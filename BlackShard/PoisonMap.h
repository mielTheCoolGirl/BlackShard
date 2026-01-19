#pragma once
#include <vector>
#include <string>
struct ShardPoisonInfo {
    int shardIndex;
    std::vector<size_t> poisonPositions; // Which bytes are poison
};

class PoisonMap {
private:
    std::vector<ShardPoisonInfo> allShards;
    std::string mapFileName; // e.g., "poison_map.bin"

public:
    PoisonMap(std::string filename) : mapFileName(filename) {}

    void addShardInfo(int shardIndex, const std::vector<size_t>& positions);

    std::vector<size_t> getPoisonPositions(int shardIndex);

    bool saveToFile();

    bool loadFromFile();
};