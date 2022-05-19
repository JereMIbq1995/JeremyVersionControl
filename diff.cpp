#include <iostream>
#include <fstream>
using namespace std;

int main() {

    bool different = false;

    ifstream fin1;
    ifstream fin2;
    
    fin1.open("20200104_094655.jpg", ios::in | ios::binary);
    fin2.open("20200104_094655_copy.jpg", ios::in | ios::binary);

    char c1 = ' ';
    char c2 = ' ';
    
    fin1.seekg(0, fin1.end);
    fin2.seekg(0, fin2.end);
    int size1 = fin1.tellg();
    int size2 = fin2.tellg();
    cout << "size1 - size2:  " << size1 << " - " << size2 << endl;

    if (size1 != size2) {
        different = true;
    }
    else {
        fin1.seekg(0, fin1.beg);
        fin2.seekg(0, fin2.beg);

        while (fin1.get(c1) && fin2.get(c2))
        {
            if (c1 != c2)
            {
                different = true;
                break;
            }
        }
    }
    
    if (different) {
        cout << "DIFFERENT!" << endl;
    }
    else {
        cout << "SAME!" << endl;
    }
}