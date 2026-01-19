#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "PoisonMap.h"
class Assembler {
private:
    std::string outputFileName;
    int totalShards;
    PoisonMap* poisonMap;

public:
    Assembler(std::string outputName, int shardCount)
        : outputFileName(outputName), totalShards(shardCount) {}
    Assembler(std::string outputName, int shardCount, PoisonMap* map)
        : outputFileName(outputName), totalShards(shardCount), poisonMap(map) {}


    bool merge();
    bool mergeWithPoisonRemoval();
    std::vector<char> removePoison(const std::vector<char>& poisonedData, const std::vector<size_t>& poisonPositions);
};