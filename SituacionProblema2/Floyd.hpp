#pragma once
/*
Actividad integradora 2
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <climits>
#include <math.h>
#include <iomanip>

#include "MST.hpp"

using namespace std;

//Complejidad O(n^2)
void LeerArcos(vector<vector<int>>& mat, vector<vector<int>>& p, vector<pair<int, pair<int, int>>>& conexiones, int m) {
    for (int i = 0; i < m; i++) {
        mat[i][i] = 0;
        p[i][i] = -1; //-1 significa conexion directa
        for (int j = 0; j < m; j++) {
            mat[i][j] = mat[j][i] = INT_MAX;
            p[i][j] = p[j][i] = -1;
        }
    }


    for (int i = 0; i < m; i++) {
        mat[conexiones[i].second.first][conexiones[i].second.second] = mat[conexiones[i].second.second][conexiones[i].second.first] = conexiones[i].first;
    }
}

//Complejidad O(n^3)
void Floyd(vector<vector<int>>& mat, vector<vector<int>>& p, int n) {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][k] != INT_MAX &&
                    mat[k][j] != INT_MAX &&
                    mat[i][k] + mat[k][j] < mat[i][j]) {
                    mat[i][j] = mat[i][k] + mat[k][j];
                    p[i][j] = k;
                }
            }
        }
    }
}

//Complejidad O(n^2)
void Consultas(vector<vector<int>>& mat, vector<vector<int>>& p, vector<Colonia>& centrales, unordered_map<string, int>& colindex) {
    cout << "Parte 3 ---------------------------------------------------------" << endl;
    for (int i = 0; i < centrales.size(); i++) {
        for (int j = i + 1; j < centrales.size(); j++) {
            if (mat[colindex[centrales[i].nombre]][colindex[centrales[j].nombre]] == INT_MAX) {
                cout << "no path" << endl;
            }
            else {
                cout << centrales[i].nombre << "-" << centrales[j].nombre << ": ";
                cout << mat[colindex[centrales[i].nombre]][colindex[centrales[j].nombre]] << endl;
            }
        }
    }
}

//Complejidad O(n^3)
void FloydResult(vector<pair<int, pair<int, int>>> conexiones, int n, int m, vector<Colonia> colonias, unordered_map<string, int> colindex) {
    vector<vector<int>> mat(m, vector<int>(m));
    vector<vector<int>> p(m, vector<int>(m));

    LeerArcos(mat, p, conexiones, m);

    Floyd(mat, p, n);
    vector<Colonia> centrales;
    for (int i = 0; i < colonias.size(); i++) {
        if (colonias[i].esCentral == 1) {
            centrales.push_back(colonias[i]);
        }
    }
    Consultas(mat, p, centrales, colindex);
}
