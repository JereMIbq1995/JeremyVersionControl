#ifndef JVC_SAVE
#define JVC_SAVE

#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <vector>
#include <set>
#include "BlobCreator.h"
#include "FileEntry.h"
#include "JvcDao.h"
#include "IndexSupplier.h"
using namespace std;



class JvcSave {

private:
    IndexSupplier idxSplr;
    BlobCreator blobCreator;
    JvcDao jvcDao;

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

    // treeIndex and treeName are the same thing
    void traverseAll(filesystem::path currentPath, int treeIndex, const set<string> &ignores)
    {

        // create a TREE object that contains all trees and blobs immediately under the current path
        // name the TREE object with the index
        // put that TREE object inside .jvc/obj/tree

        filesystem::directory_iterator it(currentPath);
        // fstream fout;
        // fout.open(".jvc/obj/tree/" + to_string(treeIndex), ios::out);

        vector<string> treeEntries;
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
                    // fout << "BLOB " << blobName << " " << dir_entry.path().filename().u8string() << "\n";
                    treeEntries.push_back("BLOB " + blobName + " " + dir_entry.path().filename().u8string());
                }

                // If it's a directory, create a tree for it
                // then put that tree/index/foldername as an entry in the current tree object
                else
                {
                    int subTreeIndex = idxSplr.getNextIndex(1);
                    // fout << "TREE " << subTreeIndex << " " << dir_entry.path().filename().u8string() << "\n";
                    treeEntries.push_back("TREE " + to_string(subTreeIndex) + " " + dir_entry.path().filename().u8string());
                    traverseAll(dir_entry.path(), subTreeIndex, ignores);
                }
            }
        }
        // fout.close();
        blobCreator.createTreeFromVector(to_string(treeIndex), treeEntries);
    }

    // Traverse through an existing directory:
    // If any file has changed, take a snapshot of the current directory by:
    //   - Create a new blob for any MODIFED file
    //   - Create a new tree for any directory above a modified file

    string traverseTreeWithDir(filesystem::path currentPath, string treeName, const set<string> &ignores)
    {
        // First, assume that the repo has not change, thus tree name is "na"
        vector<string> newTreeEntries;
        int changesDetected = 0;

        // Get all entries of the current tree
        map<string, FileEntry> entries;
        jvcDao.getTreeEntries(entries, treeName);

        // Get access to all entries of the current directory
        filesystem::directory_iterator it(currentPath);

        // std::cout << currentPath.u8string() << endl;
        // For anything that exists in currentPath:
        for (const filesystem::directory_entry dir_entry : it)
        {
            // If the thing is not one of the ignored ones, explore it
            filesystem::path entry_path = dir_entry.path();
            if (ignores.find(entry_path.filename().u8string()) == ignores.end())
            {

                // Try to see if the current dir_entry is one of the tree entries
                map<string, FileEntry>::iterator matchPos = entries.find(entry_path.filename().u8string());

                // NEW thing
                if (matchPos == entries.end())
                {
                    changesDetected += 1;

                    // If it's a new DIRECTORY:
                    //    - get new tree index
                    //    - call traverseAll on that directory
                    if (dir_entry.is_directory())
                    {
                        int newDirTreeIndex = idxSplr.getNextIndex(1);
                        string treeEntry = "TREE " + to_string(newDirTreeIndex) + " " + entry_path.filename().u8string();
                        newTreeEntries.push_back(treeEntry);
                        traverseAll(entry_path, newDirTreeIndex, ignores);
                    }

                    // Create a new blob for the file
                    else
                    {
                        cout << "Saving " << entry_path.u8string() << " ...\n";
                        string blobName = blobCreator.createBlob(entry_path.u8string());
                        string treeEntry = "BLOB " + blobName + " " + entry_path.filename().u8string();
                        newTreeEntries.push_back(treeEntry);
                    }
                }

                // If it's also in the tree:
                else
                {
                    // If it's a directory, explore it and also keep track of whether it's modified
                    if (dir_entry.is_directory())
                    {
                        string treeName = traverseTreeWithDir(entry_path, (matchPos->second).codeName, ignores);
                        changesDetected += (treeName != (matchPos->second).codeName); // If the returned treeName is different from the previous treeName, then changes have occured in this directory
                        string treeEntry = "TREE " + treeName + " " + entry_path.filename().u8string();
                        newTreeEntries.push_back(treeEntry);
                    }

                    // If it's a file, compare it to its blob. If it's different, then changesDetected
                    else if (jvcDao.diff(entry_path.u8string(), matchPos->second.codeName))
                    {
                        // cout << "Got here!" << endl;
                        changesDetected += 1;
                        cout << "Saving " << entry_path.u8string() << " ...\n";
                        string blobName = blobCreator.createBlob(entry_path.u8string());
                        string treeEntry = "BLOB " + blobName + " " + entry_path.filename().u8string();
                        newTreeEntries.push_back(treeEntry);
                    }
                    else
                    {
                        string treeEntry = "BLOB " + matchPos->second.codeName + " " + entry_path.filename().u8string();
                        newTreeEntries.push_back(treeEntry);
                    }
                }

                // std::cout << "\t" << dir_entry.path().filename().u8string() << "\t" << changesDetected << endl;
            }
        }

        // If something exists in the tree, but not in the currentPath, then something is deleted, so changes are detected.
        for (auto const &item : entries)
        {
            bool notFoundInCurrent = true;
            filesystem::directory_iterator tempDirIt(currentPath);
            for (const filesystem::directory_entry dir_entry : tempDirIt)
            {
                if (item.first == dir_entry.path().filename().u8string())
                {
                    notFoundInCurrent = false;
                    break;
                }
            }
            if (notFoundInCurrent)
            {
                changesDetected += 1;
                break;
            }
        }

        // std::cout << currentPath.u8string() << "   " << changesDetected << endl;
        // At this point, if any change is detected, create the tree using the vector created
        if (changesDetected > 0)
        {
            // cout << "creating new tree............." << endl;
            int newTreeName = idxSplr.getNextIndex(1);
            blobCreator.createTreeFromVector(to_string(newTreeName), newTreeEntries);
            return to_string(newTreeName);
        }

        return treeName;
    }

    void saveChanges(filesystem::path currentPath, const set<string> &ignores)
    {

        // For now there's only master. However, this should change
        // based on watch the current branch is.

        // If there is no version tracker, that means all files are untracked
        if (!filesystem::exists("./.jvc/head/master"))
        {
            int versionIndex = idxSplr.getNextIndex(0);
            int treeIndex = idxSplr.getNextIndex(1);
            traverseAll(currentPath, treeIndex, ignores);

            // Creating first version object
            jvcDao.createVersionObject(to_string(versionIndex), "NULL", to_string(treeIndex), "Initial save");

            // Create the head file for branch master
            jvcDao.initHead("master", to_string(versionIndex));
        }

        // If the version tracker for the current branch exists
        else
        {
            // TODO: traverse and compare file to its blob...

            // Read the most recent version's hashed name
            string mrVersionIndex = jvcDao.getHead("master");

            if (mrVersionIndex != "NULL")
            {
                // TODO: Get the tree from the version object
                ifstream versionObjIn;
                versionObjIn.open("./.jvc/obj/version/" + mrVersionIndex);

                if (versionObjIn.is_open())
                {
                    string parent;
                    string treeName;
                    versionObjIn >> parent;
                    versionObjIn >> treeName;

                    // TODO: Traverse the tree and the current directory together
                    string newTreeName = traverseTreeWithDir(currentPath, treeName, ignores);
                    if (newTreeName != treeName)
                    {
                        // Create new version object using the given treeName
                        int newVersionIndex = idxSplr.getNextIndex(0);

                        // Creating a new version object
                        jvcDao.createVersionObject(to_string(newVersionIndex), mrVersionIndex, newTreeName, "Version " + newVersionIndex);

                        // Update head/master
                        jvcDao.updateHead("master", to_string(newVersionIndex));
                    }
                    else
                    {
                        cout << "No changes detected!" << endl;
                    }
                }
                else
                {
                    cout << "Could not open version object" << endl;
                }
                versionObjIn.close();
            }
            else
            {
                cout << "Could not open branch head file" << endl;
            }
        }
    }

public:
    JvcSave() {

    }
    ~JvcSave() {

    }

    void execute() {
        
        if (!std::filesystem::exists(".jvc"))
        {
            std::cout << "Error : Not a jvc repository\n";
            return;
        }
        
        // Prepare the ignores set
        set<string> ignores;
        getIgnores(ignores);

        // Meat and butter of the functionality
        saveChanges(".\\", ignores);
    }
};
#endif