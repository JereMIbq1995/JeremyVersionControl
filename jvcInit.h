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
// using namespace std;

class JvcInit {
public:
    JvcInit() {}
    
    ~JvcInit() {}

    void execute()
    {
        if (std::filesystem::exists(".jvc")) {
            std::cout << "Error : Already a repository\n";
            return;
        }

        // creates .jvc folder and its sub-folders
        std::filesystem::create_directory(".jvc");
        SetFileAttributesA(".jvc", FILE_ATTRIBUTE_HIDDEN);

        // Creating index supplier for versions and trees
        std::filesystem::create_directory(".jvc/idxSup");
        std::fstream fout;
        fout.open(".jvc/idxSup/version", std::ios::out);
        fout << 0;
        fout.close();
        fout.open(".jvc/idxSup/tree", std::ios::out);
        fout << 0;
        fout.close();

        // Create a directory that keep track of most recent commit
        // of each branch
        std::filesystem::create_directory(".jvc/head");

        // Create a firectory to keep track of all db objects
        std::filesystem::create_directory(".jvc/obj");
        std::filesystem::create_directory(".jvc/obj/version");
        std::filesystem::create_directory(".jvc/obj/tree");
        std::filesystem::create_directory(".jvc/obj/blob");

        std::cout << "Initialized current directory as a jvc repository." << std::endl;
    }
};
