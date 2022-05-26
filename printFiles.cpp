/*
    Prints out everything in the current directory
*/
#include <iostream>
#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

void displayDirectory(filesystem::path current, int tabs) {

    filesystem::directory_iterator myIterator(current);

    for (const filesystem::directory_entry &dir_entry : myIterator)
    {
        std::string path_string(dir_entry.path().filename().u8string());
        if (path_string != ".jvc" && path_string != ".git" && path_string != "bin" && path_string != ".vscode")
        {
            for (int i = 0; i < tabs; i++) {
                cout << "\t";
            }
            cout << dir_entry.path().filename().u8string() << endl;
            // cout << path_string << "   (" << dir_entry.path() << ")" << "\n";

            if (dir_entry.is_directory())
            {
                displayDirectory(dir_entry.path(), tabs+1);
            }
        }
    }
}

int main() {
    displayDirectory(filesystem::path(".\\"), 0);
    return 0;
}