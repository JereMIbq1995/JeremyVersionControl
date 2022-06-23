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

    void recreateTree(filesystem::path currentPath, string treeName, const set<string> &ignores)
    {
        map<string, FileEntry> entries;
        objReader.getTreeEntries(entries, treeName);

        string separator = "\\";
        if (currentPath.u8string() == ".\\")
        {
            separator = "";
        }

        for(auto const &item : entries) {
            if (item.second.type == "BLOB") {
                blobCreator.createFileFromBlob(item.second.codeName, currentPath.u8string() + separator + item.first, true);
            }
            else {
                
                recreateTree(filesystem::path(currentPath.u8string() + separator + item.first), item.second.codeName, ignores);
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

    void execute(std::string version)
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
        Version versionObj = objReader.getVersion(version);
        if (versionObj.versionIndex == "error") {
            return;
        }
        else {
            cout << "HEAD is updated to version " << version << ": '" << versionObj.message << "'\n";
            objReader.updateHead("master", version);
            recreateTree(".\\", versionObj.treeIndex, ignores);
        }
    }
};
#endif