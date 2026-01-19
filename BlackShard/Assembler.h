#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Assembler {
private:
    std::string outputFileName;
    int totalShards;

public:
    Assembler(std::string outputName, int shardCount)
        : outputFileName(outputName), totalShards(shardCount) {}

    bool merge();
};