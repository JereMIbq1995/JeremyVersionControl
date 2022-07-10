#include <iostream>
#include "JvcDao.h"
#include "Version.h"
#include "FileEntry.h"

int main() {
    // Test jvcDao
    JvcDao jvcDao;
    // Version version = jvcDao.getVersion("7");
    // std::cout << version.versionIndex << std::endl;
    // std::cout << version.parentVersion << std::endl;
    // std::cout << version.treeIndex << std::endl;
    // std::cout << version.message << std::endl;

    // Test get tree entries
    map<string, FileEntry> entries;
    jvcDao.getTreeEntries(entries, "4");
    for (auto const &mapEntry : entries) {
        cout << mapEntry.second.type << "  " << mapEntry.second.codeName << "  " << mapEntry.first << "\n";
    }

}