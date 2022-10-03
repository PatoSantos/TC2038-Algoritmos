#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


//O(n*m)
string ResolveLongestCommonSubstring(vector<vector<int>>& LCS, string string1, string string2, string T) {
    string res = "";
    int max = 0;
    int end;
    //O(n)
    for (int i = 0; i < string1.size(); i++) {
        if (string1[i] == string2[0]) {
            LCS[i][0] = 1;
            max = 1;
        }
        else {
            LCS[i][0] = 0;
        }
    }

    //O(n)
    for (int j = 0; j < string2.size(); j++) {
        if (string1[0] == string2[j]) {
            LCS[0][j] = 1;
            max = 1;
        }
        else {
            LCS[0][j] = 0;
        }
    }

    //O(n*m)
    for (int i = 1; i < string1.size(); i++) {
        for (int j = 1; j < string2.size(); j++) {
            if (string1[i] == string2[j]) {
                LCS[i][j] = LCS[i - 1][j - 1] + 1;
            }
            else {
                LCS[i][j] = 0;
            }
            if (LCS[i][j] > max) {
                max = LCS[i][j];
                end = i;
            }
        }
    }

    end = end - max + 1;
    res += T;
    for (int i = max; i > 0; i--) {
        res+= string1[end];
        end++;
    }
    res += "\n";

    return res;
}

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

pair<string, int> palindromo(string txt){
    int n, max, index;
    n = txt.size();
    bool mat[n][n] = {false};

    // La diagonal es true porque las letras solas son palindromos
    for (int i = 0; i < n; i++) {
        mat[i][i] = true;
    }

    // Checar pares iguales porque asi empiezan los palindromos
    for (int i = 0; i < n - 1; i++) {
        if (txt[i] == txt[i + 1]) {
            mat[i][i + 1] = true;
            max = 2;
            index = i;
        }
    }

    // Checar ya por palindromos
    for (int i = 3; i <= n; i++) {
        for (int j = 0; j < n - i + 1; j++) {
            int k = j + i - 1;
            if (mat[j + 1][k - 1] && txt[j] == txt[k]) {
                mat[j][k] = true;
                if (i > max) {
                    max = i;
                    index = j;
                }
            }
        }
    }
    return make_pair(txt.substr(index,max), index);
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

    //para cada archivo
    for (int i = 0; i < transmissions.size(); i++){
        //Abrir el i archivo
        ifstream transmissionStream(transmissionFiles[i]);
        //Guardar todo el txt en el array transmissions
        getline(transmissionStream, transmissions[i]);
        transmissionStream.close();
    }

    /*for (int i = 0; i < transmissions.size(); i++){
        cout << transmissions[i] << endl;
    }*/

    //Abrir archivo mcode.txt
    ifstream mcodeStream(mcodeFile);
    string code;

    //Guardar cada malicious code (cada linea) en "code" y agregarlo al array mcodes.
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
    vector<int> indicesPalindromos;
    vector<string> palindromos;
    for (int i = 0; i < transmissions.size(); i++){
        pair<string,int> temp = palindromo(transmissions[i]);
        palindromos.push_back(temp.first);
        indicesPalindromos.push_back(temp.second);
    }
    writeStr += "Palíndromo más grande:\n";
    for (int i = 0; i < transmissions.size(); i++){
        if (i != 0){
            writeStr += "----\n";
        }
        writeStr += "Transmission";
        writeStr += to_string(i+1);
        writeStr += ".txt ==> Posición: ";
        writeStr += to_string(indicesPalindromos[i]);
        writeStr += "\n";
        writeStr += palindromos[i];
        writeStr += "\n";
    }
    writeStr += "==============\n";

    //Matrices para cada comparacion de substrings
    vector<vector<int>> LCS_T1_T2 = vector<vector<int>>(transmissions[0].size(), vector<int>(transmissions[1].size()));
    vector<vector<int>> LCS_T1_T3 = vector<vector<int>>(transmissions[0].size(), vector<int>(transmissions[2].size()));
    vector<vector<int>> LCS_T2_T3 = vector<vector<int>>(transmissions[1].size(), vector<int>(transmissions[2].size()));

    //Sacar substrings
    //cout << "============" << endl;
    writeStr += "Los Substring mas largos son:\n";

    writeStr += ResolveLongestCommonSubstring(LCS_T1_T2, transmissions[0], transmissions[1], "T1-T2 ==> ");
    writeStr += ResolveLongestCommonSubstring(LCS_T1_T3, transmissions[0], transmissions[2], "T1-T3 ==> ");
    writeStr += ResolveLongestCommonSubstring(LCS_T2_T3, transmissions[1], transmissions[2], "T2-T3 ==> ");


    ofstream outFile("checking.txt");
    outFile << writeStr;
    outFile.close();

    /*/------------------- Longest common substring --------------------------------------------------

    stringstream strStream; //Leer string
    ifstream inFile; //Abrir archivos
    vector<string> textStrings; //Vector con los strings de cada archivo

    //Iterar por cada archivo y guardar los archivos como strings
    for (int i = 0; i < 3; i++) {
        inFile.open(transmissionFiles[i]);
        strStream << inFile.rdbuf();

        textStrings.push_back(strStream.str());

        strStream.clear();
        strStream.str("");
        inFile.close();
    }

    //Matrices para cada comparacion de substrings
    vector<vector<int>> LCS_T1_T2 = vector<vector<int>>(textStrings[0].size(), vector<int>(textStrings[1].size()));
    vector<vector<int>> LCS_T1_T3 = vector<vector<int>>(textStrings[0].size(), vector<int>(textStrings[2].size()));
    vector<vector<int>> LCS_T2_T3 = vector<vector<int>>(textStrings[1].size(), vector<int>(textStrings[2].size()));

    //Sacar substrings
    cout << "============" << endl;
    cout << "Los Substring mas largos son: " << endl;

    ResolveLongestCommonSubstring(LCS_T1_T2, textStrings[0], textStrings[1], "T1-T2 ==> ");
    ResolveLongestCommonSubstring(LCS_T1_T3, textStrings[0], textStrings[2], "T1-T3 ==> ");
    ResolveLongestCommonSubstring(LCS_T2_T3, textStrings[1], textStrings[2], "T2-T3 ==> ");*/
    

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