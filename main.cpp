#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool isSequence(string sequence, string transmission){
    int s = 0;
    for (int i = 0; i<transmission.length(); i++){
        if (sequence[s] == transmission[i]){
            s++;
        }
        if (s == sequence.length()){
            return true;
        }
    }
    return false;
}

vector<string> subsecuenciaMasEncontrada(string mcode, string transmission){
    vector<string> res = {"", "0"};
    string code;
    int count;
    if (mcode.length() != 1){
        for (int i = 0; i < mcode.length(); i++){
            code = mcode.substr(0,i) + mcode.substr(i+1);
            count = 0;
            for (int j = 0; j < transmission.length(); j++){
                if (transmission[j] == code[0]){
                    if (isSequence(code, transmission.substr(j))){
                        count++;
                    }
                }
            }
            if (count > stoi(res[1])){
                res[0] = code;
                res[1] = to_string(count);
            }
        }
    }
    return res;
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

    vector<string> max;
    vector<string> sub1 = subsecuenciaMasEncontrada(mcode, transmissions[0]);
    vector<string> sub2 = subsecuenciaMasEncontrada(mcode, transmissions[1]);
    vector<string> sub3 = subsecuenciaMasEncontrada(mcode, transmissions[2]);

    if (stoi(sub1[1]) >= stoi(sub2[1])){
        if (stoi(sub1[1]) >= stoi(sub3[1])){
            max = sub1;
            max.push_back("transmission1.txt");
        }
        else{
            max = sub3;
            max.push_back("transmission3.txt");
        }
    }
    else{
        if (stoi(sub2[1]) >= stoi(sub3[1])){
            max = sub2;
            max.push_back("transmission2.txt");
        }
        else{
            max = sub3;
            max.push_back("transmission3.txt");
        }
    }

    res += "La subsecuencia más encontrada es: " + max[0] + " con " + max[1] + " veces en el archivo " + max[2] + "\n";
    
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

    /*for (int i = 0; i < transmissions.size(); i++){
        cout << transmissions[i] << endl;
    }*/

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

/*
abcde

bcde
acde
abde
abce
abcd
*/