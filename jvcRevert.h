#ifndef JVC_REVERT
#define JVC_REVERT

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
#include "Version.h"
#include "jvcStatus.h"
using namespace std;

/****************************************************************************
 * JvcRevert Does NOT delete anything. It simply looks at what files and folders
 * the target version has, and recreate it. If there is an ignored file from
 * current version, it still keeps it and will show up as a new file in the
 * version reverted to if that version does not ignore that file.
 * **************************************************************************/
class JvcRevert
{

private:
    IndexSupplier idxSplr;
    BlobCreator blobCreator;
    JvcDao objReader;

    void recreateTree(filesystem::path currentPath, string targetTreeName, const set<string> &ignores, string currentTreeName = "")
    {   
        // key : value
        // fileName : FileEntry {type, codeName}
        map<string, FileEntry> targetTreeEntries;
        objReader.getTreeEntries(targetTreeEntries, targetTreeName);
        
        map<string, FileEntry> currentTreeEntries;
        if (currentTreeName != "") {
            objReader.getTreeEntries(currentTreeEntries, currentTreeName);
        }

        string separator = "\\";
        if (currentPath.u8string() == ".\\")
        {
            separator = "";
        }

        // Look at all the files in the target tree. If it's a file, rewrite the content using the blob indicated
        // If it's a directory, recursively call this function
        for (auto const &item : targetTreeEntries)
        {
            if (item.second.type == "BLOB") {
                // cout << "Recreating " << item.second.codeName << "..." << endl;
                blobCreator.createFileFromBlob(item.second.codeName, currentPath.u8string() + separator + item.first, true);
            }

            // If the current item in the target tree is a directory (tree), try to see if it's also a directory in the current tree
            // if it is NOT, then don't worry about it, just traverse without the current tree
            // if it is, then traverse with the current tree parameter, which is the codeName of the matchPos value
            else {
                map<string, FileEntry>::iterator matchPos = currentTreeEntries.find(item.first);
                if (matchPos == currentTreeEntries.end()) // folder of current tree not found in target tree
                {
                    // cout << "creating directory " << currentPath.u8string() + separator + item.first << endl;
                    filesystem::create_directory(currentPath.u8string() + separator + item.first);
                    recreateTree(filesystem::path(currentPath.u8string() + separator + item.first), item.second.codeName, ignores);
                }
                else {
                    recreateTree(filesystem::path(currentPath.u8string() + separator + item.first), item.second.codeName, ignores, (matchPos->second).codeName);
                }
            }
        }

        // If there's a file that is in the current tree but not in the target tree, delete it
        if (currentTreeName != "") {
            // cout << "Removing files from current tree " << currentTreeName << "\n";
            // cout << "current tree: " << currentTreeName << "\n";
            for (auto const &item : currentTreeEntries) {
                // cout << "\t" << item.first << "\n";
                if (targetTreeEntries.find(item.first) == targetTreeEntries.end()) {
                    // cout << "Removing " << item.first << "\n";
                    if (item.second.type == "TREE") {
                        filesystem::remove_all(currentPath.u8string() + separator + item.first);
                    }
                    else {
                        filesystem::remove(currentPath.u8string() + separator + item.first);
                    }
                }
            }
        }
    }

public:
    
    JvcRevert()
    {
    }
    ~JvcRevert()
    {
    }

    void execute(std::string targetVersion)
    {
        JvcStatus jvcStatus;
        if (jvcStatus.unsavedChangesExist()) {
            cout << "Error: Cannot revert when there are unsaved changes! Use 'jvc save' to save changes before revert.\n";
            return;
        }

        // Prepare the ignores set
        set<string> ignores;
        objReader.getIgnores(ignores);

        // Read the version object:
        Version currentVersionObj = objReader.getVersion(objReader.getHead("master"));
        Version targetVersionObj = objReader.getVersion(targetVersion);

        if (targetVersionObj.versionIndex == "error") {
            return;
        }
        else {
            recreateTree(".\\", targetVersionObj.treeIndex, ignores, currentVersionObj.treeIndex);
            objReader.updateHead("master", targetVersion);
            cout << "HEAD is updated to version " << targetVersion << ": '" << targetVersionObj.message << "'\n";
        }
    }
};
#endif