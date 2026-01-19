#include <string>
#include "Seperator.h"
#include "Assembler.h"
int main() {
    std::string targetFile = "C:\\Users\\Cyber_User\\source\\repos\\bashForge\\bashForge\\miel.txt";
    std::string resFile = "res.txt";
    size_t bytesPerShard = 10;            // 1KB shards for the prototype

    // Create the Separator instance
    Separator sharder(targetFile, bytesPerShard);

    if (sharder.split()) {
        int count = sharder.getTotalShards();

        Assembler merger(resFile, count);
        if (merger.merge()) {
            std::cout << "Verification: If " << targetFile << " and "
                << resFile << " are identical,is a success!" << std::endl;
        }
    }
}
