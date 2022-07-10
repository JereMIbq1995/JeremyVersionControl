#ifndef JVC_HISTORY
#define JVC_HISTORY
#include "JvcDao.h"
#include "Version.h"
#include <iostream>
#include <string>
#include <vector>

class JvcHistory {

private:
    JvcDao objReader;

    void traverseHistory(std::string version, std::vector<string> &history) {
        if (version == "NULL") {
            return;
        }

        history.push_back(version);
        Version versionObj = objReader.getVersion(version);
        traverseHistory(versionObj.parentVersion, history);
    }

    void displayHistory(std::vector<string> &history) {
        std::cout << "History upto curent version:\n";
        std::cout << "\t";
        int historySize = history.size();
        for (int i = historySize - 1; i >= 0; i--) {
            if (i != 0) {
                std::cout << history[i] << " --> ";
            }
            else {
                std::cout << history[i] << " (current)" << "\n";
            }
        }
    }

public:

    void execute() {
        std::vector<string> history;
        std::string currentVersion = objReader.getHead("master");
        traverseHistory(currentVersion, history);
        displayHistory(history);
    }

};

#endif