#include <string>
#include "Seperator.h"
#include "Assembler.h"
int main() {
    std::string targetFile = "C:\\Users\\Cyber_User\\Downloads\\Convertor.py";
    std::string resFile = "res.py";
    size_t bytesPerShard = 1024;            // 1KB shards for the prototype

    // Create the Separator instance
    Separator sharder(targetFile, bytesPerShard);

    if (sharder.split()) {
        int count = sharder.getTotalShards();

        // 2. MERGING PHASE
        Assembler merger(resFile, count);
        if (merger.merge()) {
            std::cout << "Verification: If " << targetFile << " and "
                << resFile << " are identical,is a success!" << std::endl;
        }
    }
}
