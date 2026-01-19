#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "PoisonMap.h"

class Separator {
private:
    std::string sourcePath;
    size_t shardSize; // Size in bytes
    int totalShards;
    int poisonBytesPerShard;
    PoisonMap* poisonMap;
public:
    // Constructor
    Separator(std::string path, size_t size)
        : sourcePath(path), shardSize(size), totalShards(0) {}

    Separator(std::string path, size_t size, int poisonBytes, PoisonMap* map)
        : sourcePath(path), shardSize(size), totalShards(0),
        poisonBytesPerShard(poisonBytes), poisonMap(map) {}
    
    // Main logic to split the file
    bool splitWithPoison();
    bool split();
    int getTotalShards() const { return totalShards; }
private:
    std::vector<size_t> generatePoisonPositions(size_t shardDataSize);

    std::vector<char> embedPoison(const std::vector<char>& cleanData,
        const std::vector<size_t>& poisonPositions);
};