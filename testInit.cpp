#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
using namespace std;

int main() {
    char commit[41];
    
    ifstream fin;
    fin.open("./.jvc/head/master");

    if (fin.is_open()) {
        fin.read(commit, 41);
        string commitName(commit);
        if (commitName == "NULL") {
            cout << "No commit yet exists!\n";
        }
        else {
            cout << "Last commit was: " << commit << "\n";
        }
    }
    else {
        cout << "Cannot open file!\n";
    }

    return 0;
}