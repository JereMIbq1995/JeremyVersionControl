#include <iostream>
#include "JvcDao.h"
#include "Version.h"

int main() {
    JvcDao jvcDao;
    Version version = jvcDao.getVersion("7");
    std::cout << version.versionIndex << std::endl;
    std::cout << version.parentVersion << std::endl;
    std::cout << version.treeIndex << std::endl;
    std::cout << version.message << std::endl;

}