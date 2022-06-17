#ifndef VERSION
#define VERSION

#include <fstream>

struct Version
{
    std::string versionIndex;
    std::string parentVersion;
    std::string treeIndex;
    std::string message;
};

#endif