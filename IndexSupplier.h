#ifndef INDEX_MANAGER
#define INDEX_MANAGER

#include <fstream>
using namespace std;

class IndexSupplier {

private:

public:
    int getNextIndex(int type)
    {

        string supplier = "version";
        if (type == 1)
        {
            supplier = "tree";
        }

        fstream fin;
        fin.open(".jvc/idxSup/" + supplier, ios::in);

        int index;
        fin >> index;
        fin.close();

        fstream fout;
        fout.open(".jvc/idxSup/" + supplier, ios::out | ios::trunc);
        fout << (index + 1);
        fout.close();

        return index;
    }
};

#endif