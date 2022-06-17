#ifndef JVC_DAO
#define JVC_DAO
#include <fstream>
#include <filesystem>
#include <string>
#include "FileEntry.h"
#include "Version.h"
using namespace std;

class JvcDao {
public:
    // Ignores
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

    // compare
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
    
    // Head
    void initHead(string branch, string versionIndex) {
        if (filesystem::exists("./.jvc/head/" + branch))
        {
            fstream fout;
            fout.open("./.jvc/head/" + branch, ios::out);
            fout << versionIndex;
            fout.close();
        }
        else {
            cout << "Error: head file for branch " << branch << " already exists!\n";
        }
    }

    string getHead(string branch) {
        ifstream fin;
        fin.open("./.jvc/head/" + branch);
        if (fin.is_open()) {
            string mrVersionIndex;
            fin >> mrVersionIndex;
            fin.close();
            return mrVersionIndex;
        }
        else {
            return "NULL";
        }
    }

    void updateHead(string branch, string newVersion) {
        fstream fout;
        if (fout.is_open()) {
            fout.open("./.jvc/head/" + branch, ios::out | ios::trunc);
            fout << newVersion;
        }
        else {
            cout << "Error: could not open head file for branch '" << branch << "'.\n";
        }
        fout.close();
    }

    // Versions
    void createVersionObject(string versionIndex, string parentVersion, string treeIndex, string message) {
        fstream fout;
        fout.open("./.jvc/obj/version/" + versionIndex, ios::out);
        if (fout.is_open()) {
            fout << parentVersion << "\n"
                << treeIndex << "\n"
                << message;
        }
        else {
            cout << "Error: could not open version object for version '" << versionIndex << "'.\n";
        }
        fout.close();
    }

    Version getVersion(string versionIndex) {
        
        Version version;

        ifstream fin;
        fin.open("./.jvc/obj/version/" + versionIndex);
        if (fin.is_open()) {
            version.versionIndex = versionIndex;
            fin >> version.parentVersion;
            fin >> version.treeIndex;
            getline(fin, version.message);
            fin.ignore();
        }
        else {
            cout << "Error: could not open version object for version '" << versionIndex << "'.\n";
        }
        
    }
    
    
    // Trees
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