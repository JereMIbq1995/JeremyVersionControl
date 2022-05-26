#include <iostream>
#include <string>
#include <fstream>
#include "HashingUnit.h"
using namespace std;

void createBlob(string fileName, string blobName, char objectType) {
    
    fstream fout;
    fstream fin;

    fout.open(".jvc/obj/blob/" + blobName, ios::out | ios::binary);
    fin.open(fileName, ios::in | ios::binary);
    
    char* objType = (char*)&objectType;
    // cout << objType[0] << endl;
    if (fout.is_open() && fin.is_open()) {
        fout.write(objType, 1);
        char* oneByte;
        while (fin.read(oneByte, 1)) {
            fout.write(oneByte, 1);
        }
    }
    else {
        cout << "File does not exist or failed to open blob" << "\n";
    }
    fout.close();
    fin.close();
}

void createFileFromBlob(string blobName, string fileName)
{

    fstream fout;
    fstream fin;

    fin.open(".jvc/obj/blob/" + blobName, ios::in | ios::binary);
    fout.open(fileName, ios::out | ios::binary);

    if (fout.is_open() && fin.is_open())
    {
        char objType[2];
        fin.read(objType, 1);
        if (objType[0] == 'b') {
            cout << "Object Type: BLOB" << "\n";
        }

        cout << "Writing to file " << fileName << " from blob " << blobName << endl;
        char c[1];
        
        while (fin.read(c, 1))
        {
            fout.write(c, 1);
        }
    }
    else
    {
        cout << "Blob does not exist or failed to open file"
             << "\n";
    }
    fout.close();
    fin.close();
}

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
    
    // Get the fileName of the file the user would like to hash
    string fileName;
    cout << "Enter the name of the file: ";
    getline(cin, fileName);

    string fileDigest = hashingUnit.hashFileContent(fileName);
    cout << "Hashed content of '" << fileName << "' is: " << fileDigest << "\n";

    char objType = 'b';
    createBlob(fileName, fileDigest, objType);

    string copyFileName = getCopyFileName(fileName);
    createFileFromBlob(fileDigest, copyFileName);
} 