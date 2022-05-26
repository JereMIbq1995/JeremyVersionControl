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

    filesystem::create_directory(".jvc/idxSup");

    filesystem::create_directory(".jvc/head");
    
    filesystem::create_directory(".jvc/obj");
    filesystem::create_directory(".jvc/obj/save");
    filesystem::create_directory(".jvc/obj/tree");
    filesystem::create_directory(".jvc/obj/blob");

    // Initialize the .jvc/head/master file (OR we can just leave it):
    // char firstCommit[41] = "NULL\0";

    // ofstream fout;
    // fout.open("./.jvc/head/master");

    // if (fout.is_open()) {
    //     fout.write(firstCommit, 41);
    // }
    // else {
    //     cout << "Cannot open file!\n";
    // }

    return 0;
}