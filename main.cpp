
// Neccessities
#include <iostream>
#include <string>
#include <filesystem>

// Functionalities
#include "jvcInit.h"
#include "jvcStatus.h"
#include "jvcSave.h"


void init() {
    JvcInit jvcInitializer;
    jvcInitializer.execute();
}

void status() {
    JvcStatus jvcStatusDisplayer;
    jvcStatusDisplayer.execute();
}

void save() {
    JvcSave jvcSaver;
    jvcSaver.execute();
}

void revert(int argc, char* argv[]) {
    
    if (!std::filesystem::exists(".jvc")) {
        std::cout << "Error: Not a jvc repository.\n";
        return;
    }

    if (argc < 3) {
        std::cout << "Usage: jvc revert <version-index>. Use 'jvc history' to look up version indices.\n";
        return;
    }
    
    std::string version(argv[2]);
    std::cout << "Reverting to version " << version << "...\n";
}

void history() {
    std::cout << "Showing history...\n";
}

void execute(int argc, char* argv[]) {
    std::string command(argv[1]);

    if (command == "init") {
        init();
    }
    else if (command == "status")
    {
        status();
    }
    else if (command == "save")
    {
        save();
    }
    else if (command == "revert")
    {
        revert(argc, argv);
    }
    else if (command == "history") {
        history();
    }
    else {
        std::cout << "Error: Command '" << command << "' does not exist!\n";
    }
}

void displayUsage() {
    std::cout << "Usage...." << std::endl;
}

int main(int argc, char* argv[]) {
    
    if (argc <= 1) {
        displayUsage();
    }
    else {
        execute(argc, argv);
    }

    return 0;
}