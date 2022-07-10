
// Neccessities
#include <iostream>
#include <string>
#include <filesystem>

// Functionalities
#include "jvcInit.h"
#include "jvcStatus.h"
#include "jvcSave.h"
#include "jvcRevert.h"
#include "jvcHistory.h"


bool notJvcRepo() {
    if (!std::filesystem::exists(".jvc"))
    {
        std::cout << "Error : Not a jvc repository\n";
        return true;
    }
    return false;
}

void init() {
    JvcInit jvcInitializer;
    jvcInitializer.execute();
}

void status() {
    if (notJvcRepo()) {
        return;
    }
    JvcStatus jvcStatusDisplayer;
    jvcStatusDisplayer.execute();
}

void save(int argc, char* argv[]) {
    if (notJvcRepo()) {
        return;
    }

    JvcSave jvcSaver;
    if (argc == 3) {
        jvcSaver.execute(std::string(argv[2]));
    }
    else if (argc == 2) {
        jvcSaver.execute();
    }
    else {
        std::cout << "Usage: jvc save <optional-message>. The optional message is a string wrapped inside double quotes.\n";
        return;
    }
}

void revert(int argc, char* argv[]) {

    if (notJvcRepo()) {
        return;
    }

    if (argc < 3) {
        std::cout << "Usage: jvc revert <version-index>. Use 'jvc history' to look up version indices.\n";
        return;
    }
    
    JvcRevert jvcReverter;
    jvcReverter.execute(std::string(argv[2]));
}

void history() {
    if (notJvcRepo()) {
        return;
    }
    JvcHistory jvcHistoryExplorer;
    jvcHistoryExplorer.execute();
}

void displayUsage() {
    std::cout << "Usage...." << std::endl;
}

int main(int argc, char* argv[]) {
    
    if (argc <= 1) {
        displayUsage();
    }
    else {
        std::string command(argv[1]);

        if (command == "init")
        {
            init();
        }
        else if (command == "status")
        {
            status();
        }
        else if (command == "save")
        {
            save(argc, argv);
        }
        else if (command == "revert")
        {
            revert(argc, argv);
        }
        else if (command == "history")
        {
            history();
        }
        else
        {
            std::cout << "Error: Command '" << command << "' does not exist!\n";
        }
    }

    return 0;
}