#ifndef BLOB_CREATOR
#define BLOB_CREATOR

#include <string>
#include <filesystem>
#include "HashingUnit.h"
using namespace std;

class BlobCreator {
private:
    HashingUnit hashingUnit;

public:
    BlobCreator() {

    }

    string createBlob(string fileName) {

        // get the name of the blob by hashing the file's content
        string blobName = hashingUnit.hashFileContent(fileName);
        hashingUnit.reset();

        // Only create the blob if it does not exist
        if (!filesystem::exists("./.jvc/obj/blob/" + blobName)) {
            fstream fout;
            fstream fin;
            fout.open(".jvc/obj/blob/" + blobName, ios::out | ios::binary);
            fin.open(fileName, ios::in | ios::binary);

            // char *objType = (char *)&objectType;
            // cout << objType[0] << endl;
            if (fout.is_open() && fin.is_open())
            {
                // fout.write(objType, 1);
                char oneByte[1];
                while (fin.read(oneByte, 1))
                {
                    fout.write(oneByte, 1);
                }
            }
            else
            {
                printf("File does not exist or failed to open blob\n");
            }
            fout.close();
            fin.close();
        }

        return blobName;
    }

    void createFileFromBlob(string blobName, string filePath, bool overwrite = false) {
        fstream fout;
        fstream fin;

        fin.open(".jvc/obj/blob/" + blobName, ios::in | ios::binary);
        if (overwrite)
        {
            fout.open(filePath, ios::out | ios::binary | ios::trunc);
        }
        else {
            fout.open(filePath, ios::out | ios::binary);
        }

        if (fout.is_open() && fin.is_open())
        {
            // cout << "Writing to file " << fileName << " from blob " << blobName << endl;
            char c[1];

            while (fin.read(c, 1))
            {
                fout.write(c, 1);
            }
        }
        else
        {
            if (!fout.is_open()) {
                cout << "fout failed " + filePath << endl;
            }
            if (!fin.is_open()) {
                cout << "fin failed. file path: .jvc/obj/blob/" + blobName + "\n";
            }
            printf("Blob does not exist or failed to open file\n");
        }
        fout.close();
        fin.close();
    }

    void createTreeFromVector(string treeName, const vector<string> entries) {
        fstream fout;
        fout.open(".jvc/obj/tree/" + treeName, ios::out);
        
        for (auto entry : entries) {
            fout << entry << "\n";
        }
        fout.close();
    }

};

#endif