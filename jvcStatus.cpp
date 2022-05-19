#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace std;

void traverseAll(filesystem::path currentPath, vector<string> &untrackedFiles) {

    filesystem::directory_iterator it(currentPath);
    
    for (const filesystem::directory_entry &dir_entry : it) {
        if (!dir_entry.is_directory()) {
            untrackedFiles.push_back(dir_entry.path().u8string());
        }
        else {
            traverseAll(dir_entry.path(), untrackedFiles);
        }
    }
}

void traverseTreeWithDir(filesystem::path currentPath, string treeHashName, vector<string> &untrackedFiles) {
    // TODO: Implement this...
}

void getStatus(filesystem::path currentPath, vector<string> &untrackedFiles)
{

    // For now there's only master. However, this should change
    // based on watch the current branch is.

    // If there is no commit tracker, that means all files are untracked
    if (!filesystem::exists("./.jvc/head/master")) {
        traverseAll(currentPath, untrackedFiles);
    }

    // If the commit tracker for the current branch exists
    else {
        // TODO: traverse and compare file to its blob...
        ifstream fin;
        fin.open("./.jvc/head/master");
        

        if (fin.is_open()) {

            // Read the most recent commit's hashed name
            char commitHashName[41];
            fin.read(commitHashName, 41);
            
            // TODO: Get the tree from the commit object
            string treeHashedName = "";

            // TODO: Traverse the tree and the current directory together
            traverseTreeWithDir(filesystem::path(currentPath), treeHashedName, untrackedFiles);
        }

        fin.close();
    }
}

int main() {
    vector<string> untrackedFiles;
    getStatus(".\\", untrackedFiles);

    cout << "Untracked files:\n";
    for (const string untrackedFile : untrackedFiles) {
        cout << "\t\033[31m new: " << untrackedFile << "\033[0m\n";
    }

    return 0;
}