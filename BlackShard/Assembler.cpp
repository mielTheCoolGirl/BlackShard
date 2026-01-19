#include "Assembler.h"

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
