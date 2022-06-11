#ifndef OBJECT_READER
#define OBJECT_READER
#include <fstream>
#include <string>
#include "FileEntry.h"
using namespace std;

class ObjectReader {
public:
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

    bool diff(string filePath, string blobName)
    {

        ifstream fin1;
        ifstream fin2;

        fin1.open(filePath, ios::in | ios::binary);
        fin2.open(".jvc/obj/blob/" + blobName, ios::in | ios::binary);

        char c1 = ' ';
        char c2 = ' ';

        fin1.seekg(0, fin1.end);
        fin2.seekg(0, fin2.end);
        int size1 = fin1.tellg();
        int size2 = fin2.tellg();
        // cout << "size1 - size2:  " << size1 << " - " << size2 << endl;

        if (size1 != size2)
        {
            return true;
        }
        else
        {
            fin1.seekg(0, fin1.beg);
            fin2.seekg(0, fin2.beg);

            while (fin1.get(c1) && fin2.get(c2))
            {
                if (c1 != c2)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void getTreeEntries(map<string, FileEntry> &entries, string treeName)
    {
        fstream fin;
        fin.open(".jvc/obj/tree/" + treeName);
        string entryName;
        FileEntry entry;

        if (fin.is_open())
        {
            while (fin >> entry.type)
            {
                fin >> entry.codeName;
                fin >> entryName;

                entries.insert(pair(entryName, entry));
            }
        }
        else
        {
            cout << "Could not open file " << treeName << endl;
        }
        fin.close();
    }
};

#endif