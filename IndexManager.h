#ifndef INDEX_MANAGER
#define INDEX_MANAGER

#include <fstream>
using namespace std;

class IndexManager {

private:

public:
    int getNextCommitIndex() {
        int commitIndex = -1;
        
        // Read from the commit index supplier
        fstream fin;
        fin.open(".jvc/idxSup/commit", ios::in);
        if (fin.is_open())
        {
            fin >> commitIndex;
        }
        fin.close();

        // Write the next index to the commit supplier
        fstream fout;
        fout.open("commit", ios::out | ios::trunc);
        if (fout.is_open()) {
            fout << (commitIndex + 1);
        }
        fout.close();

        return commitIndex;
    }

    int getNextCommitIndex()
    {
        int commitIndex = -1;

        // Read from the commit index supplier
        fstream fin;
        fin.open(".jvc/idxSup/commit", ios::in);
        if (fin.is_open())
        {
            fin >> commitIndex;
        }
        fin.close();

        // Write the next index to the commit supplier
        fstream fout;
        fout.open("commit", ios::out | ios::trunc);
        if (fout.is_open())
        {
            fout << (commitIndex + 1);
        }
        fout.close();

        return commitIndex;
    }
};

#endif