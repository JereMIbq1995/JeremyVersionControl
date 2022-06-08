#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <vector>
#include <set>
#include "BlobCreator.h"
using namespace std;

BlobCreator blobCreator;

// type: 1 = tree, anything else = version
// get the next usable index from the appropriate index supplier
// then increment the index in the appropriate supplier
int getNextIndex(int type) {
    
    string supplier = "version";
    if (type == 1) {
        supplier = "tree";
    }

    fstream fin;
    fin.open(".jvc/idxSup/" + supplier, ios::in);
    
    int index;
    fin >> index;
    fin.close();

    fstream fout;
    fout.open(".jvc/idxSup/" + supplier, ios::out | ios::trunc);
    fout << (index + 1);
    fout.close();

    return index;
}

void traverseAll(filesystem::path currentPath, const set<string> &ignores, int treeIndex)
{

    // create a TREE object that contains all trees and blobs immediately under the current path
    // name the TREE object with the index
    // put that TREE object inside .jvc/obj/tree

    filesystem::directory_iterator it(currentPath);
    fstream fout;
    fout.open(".jvc/obj/tree/" + to_string(treeIndex), ios::out);
    
    for (const filesystem::directory_entry &dir_entry : it)
    {
        if (ignores.find(dir_entry.path().filename().u8string()) == ignores.end())
        {
            cout << "Saving " << dir_entry.path().u8string() << " ...\n";
            // If it's a file, not a directory, create a blob for it
            // then put that blob/hash/filename as an entry in the current tree object
            if (!dir_entry.is_directory())
            {
                string blobName = blobCreator.createBlob(dir_entry.path().u8string());
                fout << "BLOB " << blobName << " " << dir_entry.path().filename().u8string() << "\n";
            }

            // If it's a directory, create a tree for it
            // then put that tree/index/foldername as an entry in the current tree object
            else
            {
                int treeIndex = getNextIndex(1);
                fout << "TREE " << treeIndex << " " << dir_entry.path().filename().u8string() << "\n";
                traverseAll(dir_entry.path(), ignores, treeIndex);
            }
        }
    }
    fout.close();
}

void traverseTreeWithDir(filesystem::path currentPath, string treeName, const set<string> &ignores)
{
    // TODO: Implement this...
}

void saveChanges(filesystem::path currentPath, const set<string> &ignores)
{

    // For now there's only master. However, this should change
    // based on watch the current branch is.

    // If there is no version tracker, that means all files are untracked
    if (!filesystem::exists("./.jvc/head/master"))
    {
        int versionIndex = getNextIndex(0);
        int treeIndex = getNextIndex(1);
        traverseAll(currentPath, ignores, treeIndex);
        fstream fout;
        fout.open("./.jvc/obj/version/" + to_string(versionIndex), ios::out);
        fout << "NULL\n" << treeIndex << "\n" << "Initial save";
        fout.close();

        fout.open("./.jvc/head/master", ios::out);
        fout << versionIndex;
        fout.close();
    }

    // If the version tracker for the current branch exists
    else
    {
        // TODO: traverse and compare file to its blob...
        ifstream fin;
        fin.open("./.jvc/head/master");

        if (fin.is_open())
        {

            // Read the most recent version's hashed name
            int versionIndex;
            fin.read((char *)&versionIndex, 4);

            // TODO: Get the tree from the version object
            string treeName = "";

            // TODO: Traverse the tree and the current directory together
            traverseTreeWithDir(currentPath, treeName, ignores);
        }

        fin.close();
    }
}

void getIgnores(set<string> &ignores)
{
    if (filesystem::exists("./.jvcIgnore"))
    {
        ifstream fin;
        fin.open("./.jvcIgnore");

        string fileName;
        while (getline(fin, fileName))
        {
            ignores.insert(fileName);
        }
        fin.close();
    }
}

int main()
{
    // Prepare the ignores set
    set<string> ignores;
    getIgnores(ignores);

    // Meat and butter of the functionality
    saveChanges(".\\", ignores);

    return 0;
}