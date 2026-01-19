#include <string>
#include "Seperator.h"
#include "Assembler.h"
#include "PoisonMap.h"

int main() {
    std::string targetFile = "C:\\Users\\Cyber_User\\source\\repos\\bashForge\\bashForge\\miel.txt";
    std::string resFile = "res.txt";
    size_t bytesPerShard = 10;           // 10 bytes per shard
    int poisonBytesPerShard = 3;         // Add 3 poison bytes per shard

    PoisonMap poisonMap("poison_map.bin");
    std::cout << "shredding and embedding poison..." << std::endl;
    Separator sharder(targetFile, bytesPerShard, poisonBytesPerShard, &poisonMap);

    if (sharder.splitWithPoison()) {
        int count = sharder.getTotalShards();
        std::cout << "Created " << count << " poisoned shards" << std::endl << std::endl;

        if (poisonMap.saveToFile()) 
        {
            std::cout << "Poison map saved successfully" << std::endl << std::endl;
        }

        std::cout << "loading poison map and reassembling..." << std::endl;


        PoisonMap loadedMap("poison_map.bin");
        if (loadedMap.loadFromFile()) 
        {
            Assembler merger(resFile, count, &loadedMap);
            if (merger.mergeWithPoisonRemoval()) 
            {
                std::cout << "Verification: Compare " << targetFile << " and " << resFile << std::endl;
                std::cout << "If identical, WP2 is working correctly!" << std::endl;
            }
        }
    }

    return 0;
}