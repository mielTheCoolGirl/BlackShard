#include "Seperator.h"

bool Separator::split()
{
    std::ifstream file(sourcePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open source file." << std::endl;
        return false;
    }

    // Buffer to hold one shard's worth of data
    std::vector<char> buffer(shardSize);

    while (file) {
        // Read 'shardSize' bytes into the buffer
        file.read(buffer.data(), shardSize);
        std::streamsize bytesRead = file.gcount(); // Check how many bytes were actually read

        if (bytesRead > 0) {
            // Generate a unique name for this shard (e.g., shard_0.bin)
            std::string shardName = "shard_" + std::to_string(totalShards) + ".bin";

            std::ofstream outShard(shardName, std::ios::binary);
            if (outShard.is_open()) {
                outShard.write(buffer.data(), bytesRead);
                outShard.close();
                totalShards++;
            }
        }
    }

    file.close();
    std::cout << "Successfully created " << totalShards << " shards." << std::endl;
    return true;
}

