#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<string> transmissionFiles = {"transmission1.txt", "transmission1.txt", "transmission1.txt"};
    string mcodeFile = "mcode.txt";

    vector<string> transmissions(transmissionFiles.size());
    vector<string> mcodes;

    for (int i = 0; i < transmissions.size(); i++){
        ifstream transmissionStream(transmissionFiles[i]);
        getline(transmissionStream, transmissions[i]);
        transmissionStream.close();
    }

    ifstream mcodeStream(mcodeFile);
    string code;

    while (getline(mcodeStream,code)){
        mcodes.push_back(code);
    }
    mcodeStream.close();

    for (int i = 0; i < transmissions.size(); i++){
        cout << transmissions[i] << endl;
    }

    for (int i = 0; i < mcodes.size(); i++){
        cout << mcodes[i] << endl;
    }

    return 0;
}
