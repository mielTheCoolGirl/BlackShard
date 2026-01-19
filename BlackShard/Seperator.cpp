#include "Seperator.h"

bool Separator::splitWithPoison()
{
    if (poisonMap == nullptr) 
    {
        std::cerr << "error: no poison map provided." << std::endl;
        return false;
    }

    std::ifstream file(sourcePath, std::ios::binary);
    if (!file.is_open()) 
    {
        std::cerr << "error: couldn't open file." << std::endl;
        return false;
    }

    std::vector<char> buffer(shardSize);

    while (file) 
    {  
        file.read(buffer.data(), shardSize);
        std::streamsize bytesRead = file.gcount();

        if (bytesRead > 0) 
        {
            //resize buff to actual bytes being read
            std::vector<char> cleanData(buffer.begin(), buffer.begin() + bytesRead);

            //generate random poison positions
            std::vector<size_t> poisonPositions = generatePoisonPositions(cleanData.size());

            //put poison bytes into the clean data
            std::vector<char> poisonedData = embedPoison(cleanData, poisonPositions);

            //save poisoned shard to file
            std::string shardName = "shard_" + std::to_string(totalShards) + ".bin";
            std::ofstream outShard(shardName, std::ios::binary);
            if (outShard.is_open()) 
            {
                outShard.write(poisonedData.data(), poisonedData.size());
                outShard.close();
                poisonMap->addShardInfo(totalShards, poisonPositions);
                totalShards++;
            }
        }
    }

    file.close();
    std::cout << "Successfully created " << totalShards << " poisoned shards." << std::endl;
    return true;
}

std::vector<size_t> Separator::generatePoisonPositions(size_t shardDataSize)
{
    std::vector<size_t> positions;

    if (poisonBytesPerShard <= 0 || shardDataSize == 0) //if there's no need to add poison
    {
        return positions; 
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    //calc max position (poison will be inserted AFTER clean data)
    //positions range from 0 to shardDataSize (inclusive)
    std::uniform_int_distribution<size_t> dis(0, shardDataSize);

    while (positions.size() < static_cast<size_t>(poisonBytesPerShard)) {
        size_t pos = dis(gen);

        //ensure no duplicates
        if (std::find(positions.begin(), positions.end(), pos) == positions.end()) 
        {
            positions.push_back(pos);
        }
    }

    //sort positions for easier insertion
    std::sort(positions.begin(), positions.end());

    return positions;
}


std::vector<char> Separator::embedPoison(const std::vector<char>& cleanData,
    const std::vector<size_t>& poisonPositions)
{
    std::vector<char> result;
    result.reserve(cleanData.size() + poisonPositions.size());

    //rand poison byte generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    size_t cleanIndex = 0;
    size_t poisonIndex = 0;
    size_t currentPos = 0;

    //merge clean data and poison bytes
    while (cleanIndex < cleanData.size() || poisonIndex < poisonPositions.size()) 
    {
        //check if we need to insert poison at current position
        if (poisonIndex < poisonPositions.size() && currentPos == poisonPositions[poisonIndex]) 
        {
            // Insert poison byte
            result.push_back(static_cast<char>(dis(gen)));
            poisonIndex++;
            currentPos++;
        }
 
        else if (cleanIndex < cleanData.size()) 
        {
            result.push_back(cleanData[cleanIndex]);
            cleanIndex++;
            currentPos++;
        }
    }

    return result;
}

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

