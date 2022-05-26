#ifndef BLOB_CREATOR
#define BLOB_CREATOR

#include <string>
#include "HashingUnit.h"
using namespace std;

class BlobCreator {
private:
    HashingUnit hashingUnit;

public:
    BlobCreator() {

    }

    void createBlob(string fileName) {

        // get the name of the blob by hashing the file's content
        string blobName = hashingUnit.hashFileContent(fileName);
        hashingUnit.reset();

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

    void createFileFromBlob(string blobName, string filePath) {
        fstream fout;
        fstream fin;

        fin.open(".jvc/obj/blob/" + blobName, ios::in | ios::binary);
        fout.open(filePath, ios::out | ios::binary);

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
            printf("Blob does not exist or failed to open file\n");
        }
        fout.close();
        fin.close();
    }

};

#endif