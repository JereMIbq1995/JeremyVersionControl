#include <iostream>
#include <string>
#include <fstream>
#include "HashingUnit.h"
#include "BlobCreator.h"
using namespace std;

/*
    Helper function: return the copy file name
*/
string getCopyFileName(string fileName) {
    string copyFileName = "";
    int periodPos = 0;
    for (int i = fileName.size() - 1; i >= 0; i--)
    {
        if (fileName[i] == '.')
        {
            periodPos = i;
            break;
        }
    }
    for (int i = 0; i < periodPos; i++)
    {
        copyFileName.push_back(fileName[i]);
    }
    copyFileName = copyFileName + "_re";
    for (int i = periodPos; i < fileName.size(); i++)
    {
        copyFileName.push_back(fileName[i]);
    }
    return copyFileName;
}

/*
    Takes in a filename and create a blob for its content
*/
int main() {
    // Initialize hashing unit
    HashingUnit hashingUnit;
    BlobCreator blobCreator;
    
    // Get the fileName of the file the user would like to hash
    string fileName;
    cout << "Enter the name of the file: ";
    getline(cin, fileName);

    string blobName = blobCreator.createBlob(fileName);
    cout << "Hashed content of '" << fileName << "' is: " << blobName << "\n";

    string copyFileName = getCopyFileName(fileName);
    blobCreator.createFileFromBlob(blobName, copyFileName);
} 