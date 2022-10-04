/*
Actividad integradora 1
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

//Complejidad: O(n*m)
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

//Complejidad: O(n)
bool isSequence(string sequence, string transmission){
    int s = 0;
    for (int i = 0; i<transmission.length(); i++){ //O(n)
        if (sequence[s] == transmission[i]){
            s++;
        }
        if (s == sequence.length()){
            return true;
        }
    }
    return false;
}

//Complejidad: O(n^3)
vector<string> subsecuenciaMasEncontrada(string mcode, string transmission){
    vector<string> res = {"", "0"};
    string code;
    int count;
    if (mcode.length() != 1){
        for (int i = 0; i < mcode.length(); i++){ //O(n^3)
            code = mcode.substr(0,i) + mcode.substr(i+1);
            count = 0;
            for (int j = 0; j < transmission.length(); j++){ //O(n^2)
                if (transmission[j] == code[0]){
                    if (isSequence(code, transmission.substr(j))){ //O(n)
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

//Complejidad O(n)
vector<int> z_Function(string general){
    int n = general.length();
    vector<int> result(n,0);
    for (int i=1, l=0, r=0; i<n; i++){ //O(n)
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

//Complejidad: O(n^2)
pair<string, int> palindromo(string txt){
    int n, max, index;
    n = txt.size();
    vector<vector<bool>> mat(n, vector<bool>(n, false));

    // La diagonal es true porque las letras solas son palindromos
    for (int i = 0; i < n; i++) { //O(n)
        mat[i][i] = true;
    }

    // Checar pares iguales porque asi empiezan los palindromos
    for (int i = 0; i < n - 1; i++) { //O(n)
        if (txt[i] == txt[i + 1]) {
            mat[i][i + 1] = true;
            max = 2;
            index = i;
        }
    }

    // Checar ya por palindromos
    for (int i = 3; i <= n; i++) { //O(n^2)
        for (int j = 0; j < n - i + 1; j++) { //O(n)
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

// Complejidad: O(n^3)
string buscarIncidencias(string mcode, vector<string> transmissions){
    string res = "Código: " + mcode + "\n";

    for (int i = 0; i < transmissions.size(); i++){ //O(n^2)
        vector<int> result;
        string general = mcode + "$" + transmissions[i];
        vector<int> zf = z_Function(general); //O(n)
        for (int i=0; i < zf.size(); i++){ //O(n)
            if (zf[i] == mcode.length()){
                result.push_back(i);
            }
        }
        res += "Transmission" + to_string(i+1) + ".txt ==> " + to_string(result.size()) + " veces\n";
        for (int i=0; i < result.size(); i++){ //O(n)
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
    vector<string> sub1 = subsecuenciaMasEncontrada(mcode, transmissions[0]);//O(n^3)
    vector<string> sub2 = subsecuenciaMasEncontrada(mcode, transmissions[1]);//O(n^3)
    vector<string> sub3 = subsecuenciaMasEncontrada(mcode, transmissions[2]);//O(n^3)

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
    //Se leen las transmisiones y se guardan en un arreglo.
    vector<string> transmissionFiles = {"transmission1.txt", "transmission2.txt", "transmission3.txt"};
    string mcodeFile = "mcode.txt";

    vector<string> transmissions(transmissionFiles.size()); //Arreglo con las transmisiones
    vector<string> mcodes;

    for (int i = 0; i < transmissions.size(); i++){ //O(n)
        //Abrir el i archivo
        ifstream transmissionStream(transmissionFiles[i]);
        //Guardar todo el txt en el arreglo transmissions
        getline(transmissionStream, transmissions[i]);
        transmissionStream.close();
    }


    //Abrir archivo mcode.txt
    ifstream mcodeStream(mcodeFile);
    string code;

    //Guardar cada malicious code (cada linea) en "code" y agregarlo al array mcodes.
    while (getline(mcodeStream,code)){ //O(n)
        mcodes.push_back(code);
    }
    mcodeStream.close();

    //Se crea el string inicial donde se guardaran todas las lecturas para luego
    //escribirlo a checking.txt
    string writeStr = "";

    //Se buscan las incidencias de cada mcode en todos los archivos
    for (int i = 0; i < mcodes.size(); i++){ //O(n^4)
        if (i == 0){
            writeStr += buscarIncidencias(mcodes[i],transmissions);//O(n^3)
        } else {
            writeStr += "--------------\n" + buscarIncidencias(mcodes[i],transmissions);//O(n^3)
        }
    }

    writeStr += "==============\n";

    //Se buscan los palíndromos de cada archivo
    vector<int> indicesPalindromos;
    vector<string> palindromos;
    for (int i = 0; i < transmissions.size(); i++){//O(n^3)
        pair<string,int> temp = palindromo(transmissions[i]);//O(n^2)
        palindromos.push_back(temp.first);
        indicesPalindromos.push_back(temp.second);
    }
    writeStr += "Palíndromo más grande:\n";
    for (int i = 0; i < transmissions.size(); i++){ //O(n)
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

    //Se busca el substring común más grande entre los archivos.
    //Matrices para cada comparacion de substrings
    vector<vector<int>> LCS_T1_T2 = vector<vector<int>>(transmissions[0].size(), vector<int>(transmissions[1].size()));
    vector<vector<int>> LCS_T1_T3 = vector<vector<int>>(transmissions[0].size(), vector<int>(transmissions[2].size()));
    vector<vector<int>> LCS_T2_T3 = vector<vector<int>>(transmissions[1].size(), vector<int>(transmissions[2].size()));

    //Sacar substrings
    writeStr += "Los Substring mas largos son:\n";

    writeStr += ResolveLongestCommonSubstring(LCS_T1_T2, transmissions[0], transmissions[1], "T1-T2 ==> ");//O(n*m)
    writeStr += ResolveLongestCommonSubstring(LCS_T1_T3, transmissions[0], transmissions[2], "T1-T3 ==> ");//O(n*m)
    writeStr += ResolveLongestCommonSubstring(LCS_T2_T3, transmissions[1], transmissions[2], "T2-T3 ==> ");//O(n*m)

    //Se escribe todo lo de writeStr en checking.txt
    ofstream outFile("checking.txt");
    outFile << writeStr;
    outFile.close();

    return 0;
}