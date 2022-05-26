#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

void traverseAll(filesystem::path currentPath, vector<string> &newFiles, const set<string> &ignores) {

    filesystem::directory_iterator it(currentPath);
    
    for (const filesystem::directory_entry &dir_entry : it) {
        if (ignores.find(dir_entry.path().filename().u8string()) == ignores.end()) {
            if (!dir_entry.is_directory()) {
                    newFiles.push_back(dir_entry.path().u8string());
            }
            else {
                traverseAll(dir_entry.path(), newFiles, ignores);
            }
        }
    }
}

void traverseTreeWithDir(filesystem::path currentPath, string treeName, vector<string> &newFiles, vector<string> &deletedFiles,
                         vector<string> &modifiedFiles, const set<string> &ignores)
{
    // TODO: Implement this...
}

void getStatus(filesystem::path currentPath, vector<string> &newFiles, vector<string> &deletedFiles,
                vector<string> &modifiedFiles, const set<string> &ignores)
{

    // For now there's only master. However, this should change
    // based on watch the current branch is.

    // If there is no commit tracker, that means all files are untracked
    if (!filesystem::exists("./.jvc/head/master")) {
        traverseAll(currentPath, newFiles, ignores);
    }

    // If the commit tracker for the current branch exists
    else {
        // TODO: traverse and compare file to its blob...
        ifstream fin;
        fin.open("./.jvc/head/master");
        

        if (fin.is_open()) {

            // Read the most recent commit's hashed name
            int commitIndex;
            fin.read((char*) &commitIndex, 4);
            
            // TODO: Get the tree from the commit object
            string treeHashedName = "";

            // TODO: Traverse the tree and the current directory together
            traverseTreeWithDir(filesystem::path(currentPath), treeHashedName, newFiles, deletedFiles, modifiedFiles, ignores);
        }

        fin.close();
    }
}

void getIgnores(set<string> &ignores) {
    if (filesystem::exists("./.jvcIgnore")) {
        ifstream fin;
        fin.open("./.jvcIgnore");
        
        string fileName;
        while (getline(fin, fileName)) {
            ignores.insert(fileName);
        }
        fin.close();
    }
}

int main() {

    // Prepare the 3 categories of files
    vector<string> newFiles;
    vector<string> deletedFiles;
    vector<string> modifiedFiles;

    // Prepare the ignores set
    set<string> ignores;
    getIgnores(ignores);

    // Meat and butter of the functionality
    getStatus(".\\", newFiles, deletedFiles, modifiedFiles, ignores);


    // Print out all untracked files
    cout << "Untracked files:\n";

    for (const string newFile : newFiles) {
        cout << "\t\033[31m new: " << newFile << "\033[0m\n";
    }

    return 0;
}