#include "PoisonMap.h"
#include <fstream>
#include <iostream>
#include <algorithm>

void PoisonMap::addShardInfo(int shardIndex, const std::vector<size_t>& positions)
{
    ShardPoisonInfo info;
    info.shardIndex = shardIndex;
    info.poisonPositions = positions;
    allShards.push_back(info);
}

std::vector<size_t> PoisonMap::getPoisonPositions(int shardIndex)
{
    for (const auto& shard : allShards) {
        if (shard.shardIndex == shardIndex) {
            return shard.poisonPositions;
        }
    }
    //return an emopty vector if there's nothing found
    return std::vector<size_t>();
}

bool PoisonMap::saveToFile()
{
    std::ofstream outFile(mapFileName, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create poison map file." << std::endl;
        return false;
    }

    int totalShards = static_cast<int>(allShards.size());
    outFile.write(reinterpret_cast<const char*>(&totalShards), sizeof(int));

    
    for (const auto& shard : allShards) 
    {
        outFile.write(reinterpret_cast<const char*>(&shard.shardIndex), sizeof(int));

        int numPoison = static_cast<int>(shard.poisonPositions.size());
        outFile.write(reinterpret_cast<const char*>(&numPoison), sizeof(int));

        //write all posion positions
        for (size_t pos : shard.poisonPositions) 
        {
            outFile.write(reinterpret_cast<const char*>(&pos), sizeof(size_t));
        }
    }
    outFile.close();
    std::cout << "Poison map saved to " << mapFileName << std::endl;
    return true;
}

bool PoisonMap::loadFromFile()
{
    std::ifstream inFile(mapFileName, std::ios::binary);
    if (!inFile.is_open()) 
    {
        std::cerr << "Error: Could not open poison map file." << std::endl;
        return false;
    }
    allShards.clear();

    
    int totalShards;
    inFile.read(reinterpret_cast<char*>(&totalShards), sizeof(int));

    //read info for each shard
    for (int i = 0; i < totalShards; ++i) {
        ShardPoisonInfo info;
        inFile.read(reinterpret_cast<char*>(&info.shardIndex), sizeof(int));

        int numPoison;
        inFile.read(reinterpret_cast<char*>(&numPoison), sizeof(int));

        info.poisonPositions.resize(numPoison);
        for (int j = 0; j < numPoison; ++j) 
        {
            size_t pos;
            inFile.read(reinterpret_cast<char*>(&pos), sizeof(size_t));
            info.poisonPositions[j] = pos;
        }

        allShards.push_back(info);
    }

    inFile.close();
    std::cout << "Poison map loaded from " << mapFileName << std::endl;
    return true;
}