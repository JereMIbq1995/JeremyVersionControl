/*
    jvc init:

        - creates the .jvc directory

        - creates the .jvc/head folder
        - creates the .jvc/head/master file

        - creates the .jvc/obj folder

*/
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <fstream>
#include <fileapi.h>
using namespace std;

int main() {

    // creates .jvc folder and its sub-folders
    filesystem::create_directory(".jvc");
    SetFileAttributesA(".jvc", FILE_ATTRIBUTE_HIDDEN);

    // Creating index supplier for versions and trees
    filesystem::create_directory(".jvc/idxSup");
    fstream fout;
    fout.open(".jvc/idxSup/version", ios::out);
    fout << 0;
    fout.close();
    fout.open(".jvc/idxSup/tree", ios::out);
    fout << 0;
    fout.close();

    // Create a directory that keep track of most recent commit
    // of each branch
    filesystem::create_directory(".jvc/head");

    // Create a firectory to keep track of all db objects
    filesystem::create_directory(".jvc/obj");
    filesystem::create_directory(".jvc/obj/version");
    filesystem::create_directory(".jvc/obj/tree");
    filesystem::create_directory(".jvc/obj/blob");

    return 0;
}