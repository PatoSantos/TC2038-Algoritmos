#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> subsecuenciaMasEncontrada(string mcode, string transmissions){
}

vector<int> z_Function(string general){
    int n = general.length();
    vector<int> result(n,0);
    for (int i=1, l=0, r=0; i<n; i++){
        if (i <= r){
            result[i] = min(r-i+1, result[i-1]);
        }
        while (i+result[i] < n && general[result[i]] == general[i+result[i]]){
            //cout << "f" << endl;
            result[i]++;
        }
        if (i+result[i]-1 > r){
            l=i;
            r=i+result[i] -1;
        }
    }
    return result;
}

string buscarIncidencias(string mcode, vector<string> transmissions){
    string res = "Código: " + mcode + "\n";

    for (int i = 0; i < transmissions.size(); i++){
        vector<int> result;
        string general = mcode + "$" + transmissions[i];
        vector<int> zf = z_Function(general);
        for (int i=0; i < zf.size(); i++){
            if (zf[i] == mcode.length()){
                result.push_back(i);
            }
        }
        res += "Transmission" + to_string(i+1) + ".txt ==> " + to_string(result.size()) + " veces\n";
        for (int i=0; i < result.size(); i++){
            if (i==0){
                res += to_string(result[i] - mcode.length() - 1);
            }
            else {
                res += ", " + to_string(result[i] - mcode.length() - 1);
            }
        }
        res += "\n";
    }
    
    return res;
}

int main()
{
    vector<string> transmissionFiles = {"transmission1.txt", "transmission2.txt", "transmission3.txt"};
    string mcodeFile = "mcode.txt";

    vector<string> transmissions(transmissionFiles.size());
    vector<string> mcodes;

    for (int i = 0; i < transmissions.size(); i++){
        ifstream transmissionStream(transmissionFiles[i]);
        getline(transmissionStream, transmissions[i]);
        transmissionStream.close();
    }

    for (int i = 0; i < transmissions.size(); i++){
        cout << transmissions[i] << endl;
    }

    ifstream mcodeStream(mcodeFile);
    string code;

    while (getline(mcodeStream,code)){
        mcodes.push_back(code);
    }
    mcodeStream.close();

    string writeStr = "";

    for (int i = 0; i < mcodes.size(); i++){
        if (i == 0){
            writeStr += buscarIncidencias(mcodes[i],transmissions);
        } else {
            writeStr += "--------------\n" + buscarIncidencias(mcodes[i],transmissions);
        }
    }

    writeStr += "==============\n";

    ofstream outFile("checking.txt");
    outFile << writeStr;
    outFile.close();

    return 0;
}
