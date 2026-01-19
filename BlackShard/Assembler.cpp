#include "Assembler.h"

bool Assembler::mergeWithPoisonRemoval()
{
    if (poisonMap == nullptr) {
        std::cerr << "Error: PoisonMap not provided." << std::endl;
        return false;
    }

    std::ofstream outFile(outputFileName, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create output file." << std::endl;
        return false;
    }

    for (int i = 0; i < totalShards; ++i) {
        std::string shardName = "shard_" + std::to_string(i) + ".bin";

        // Read entire poisoned shard into memory
        std::ifstream inShard(shardName, std::ios::binary);
        if (!inShard.is_open()) {
            std::cerr << "Error: Could not open " << shardName << std::endl;
            return false;
        }

        // Get file size
        inShard.seekg(0, std::ios::end);
        size_t fileSize = inShard.tellg();
        inShard.seekg(0, std::ios::beg);

        // Read all data
        std::vector<char> poisonedData(fileSize);
        inShard.read(poisonedData.data(), fileSize);
        inShard.close();

        // Get poison positions for this shard
        std::vector<size_t> poisonPositions = poisonMap->getPoisonPositions(i);

        // Remove poison bytes
        std::vector<char> cleanData = removePoison(poisonedData, poisonPositions);

        // Write clean data to output
        outFile.write(cleanData.data(), cleanData.size());

        std::cout << "Processed shard " << i << ": removed " << poisonPositions.size()
            << " poison bytes" << std::endl;
    }

    outFile.close();
    std::cout << "Successfully merged " << totalShards << " shards into "
        << outputFileName << " (poison removed)" << std::endl;
    return true;
}
std::vector<char> Assembler::removePoison(const std::vector<char>& poisonedData,
    const std::vector<size_t>& poisonPositions)
{
    std::vector<char> cleanData;
    cleanData.reserve(poisonedData.size() - poisonPositions.size());

    // Iterate through poisoned data and skip poison positions
    for (size_t i = 0; i < poisonedData.size(); ++i) {
        // Check if current position is a poison byte
        if (std::find(poisonPositions.begin(), poisonPositions.end(), i) == poisonPositions.end()) {
            // Not poison - copy to clean data
            cleanData.push_back(poisonedData[i]);
        }
        // else: skip this byte (it's poison)
    }

    return cleanData;
}
bool Assembler::merge()
{
    // Create/Overwrite the final output file in binary mode
    std::ofstream outFile(outputFileName, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create output file." << std::endl;
        return false;
    }

    for (int i = 0; i < totalShards; ++i) {
        // Construct the shard name (must match Separator's naming convention)
        std::string shardName = "shard_" + std::to_string(i) + ".bin";

        std::ifstream inShard(shardName, std::ios::binary);
        if (!inShard.is_open()) {
            std::cerr << "Error: Could not open " << shardName << std::endl;
            return false;
        }

        // Write the entire content of the shard into the output file
        outFile << inShard.rdbuf();

        inShard.close();
    }

    outFile.close();
    std::cout << "Successfully merged " << totalShards << " shards into " << outputFileName << std::endl;
    return true;
}
