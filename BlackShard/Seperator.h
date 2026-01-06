#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class Separator {
private:
    std::string sourcePath;
    size_t shardSize; // Size in bytes
    int totalShards;

public:
    // Constructor
    Separator(std::string path, size_t size)
        : sourcePath(path), shardSize(size), totalShards(0) {}

    // Main logic to split the file
    bool split();
    int getTotalShards() const { return totalShards; }
};