#pragma once
/*
Actividad integradora 2
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <iostream>
#include <climits>
#include <algorithm>
#include <vector>
#include <string>

#include "Colonia.hpp"

using namespace std;

//Complejidad O(n^2)
void LeerArcos(vector<vector<int>>& mat, vector<vector<int>>& p, vector<pair<int, pair<int, int>>>& conexiones, int m) {
    for (int i = 0; i < m; i++) {
        mat[i][i] = 0;
        p[i][i] = -1; 
        //Inicializar matriz de costos y matriz de trayectoria
        for (int j = 0; j < m; j++) {
            mat[i][j] = mat[j][i] = INT_MAX;
            p[i][j] = p[j][i] = -1;
        }
    }

    //Ponder costo en matriz de punto a -> b
    for (int i = 0; i < m; i++) {
        mat[conexiones[i].second.first][conexiones[i].second.second] = mat[conexiones[i].second.second][conexiones[i].second.first] = conexiones[i].first;
    }
}

//Complejidad O(n^3)
void Floyd(vector<vector<int>>& mat, vector<vector<int>>& p, int n) {
    //Metodo de floyd
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][k] != INT_MAX && mat[k][j] != INT_MAX && mat[i][k] + mat[k][j] < mat[i][j]) {
                    //Actualizar costo
                    mat[i][j] = mat[i][k] + mat[k][j];
                    //Actualizar la ruta
                    p[i][j] = k;
                }
            }
        }
    }
}

//Guardar y regresar la ruta
//Complejidad O(2^n)
void checaTrayectoria(vector<vector<int>>& p, int x, int y, vector<Colonia>& colonias, string& out) {
    //Si no es una conexion directa
    if (p[x][y] != -1) {
        //Checar ruta de punto x a nuevo punto
        checaTrayectoria(p, x, p[x][y], colonias, out);
       out += colonias[p[x][y]].nombre + " - "; 
        //Checar ruta de nuevo punto a punto y
        checaTrayectoria(p, p[x][y], y, colonias, out);
    }
}

string Consultas(vector<vector<int>>& mat, vector<vector<int>>& p, vector<Colonia>& centrales, unordered_map<string, int>& colindex, vector<Colonia>& colonias) {

    string out = "";
    //Imprimir todas las combinaciones de centrales que hay
    for (int i = 0; i < centrales.size(); i++) {
        for (int j = i + 1; j < centrales.size(); j++) {
            //Si la ceentral no tiene ruta a otra central dar el error
            if (mat[colindex[centrales[i].nombre]][colindex[centrales[j].nombre]] == INT_MAX) {
                cout << "no path" << endl;
            }
            else {
                //Imprimir punto de partida
                out += centrales[i].nombre + " - ";
                //Imprimir ruta
                checaTrayectoria(p,colindex[centrales[i].nombre], colindex[centrales[j].nombre], colonias, out);
                //Imprimir punto de destino
                out += centrales[j].nombre;
                //Imprimir la primera central a la segunda central
                out += " (" + to_string(mat[colindex[centrales[i].nombre]][colindex[centrales[j].nombre]]) + ")\n";
            }
        }
    }
    return out;
}

//Complejidad O(n^3)
string FloydResult(vector<pair<int, pair<int, int>>> conexiones, int n, int m, vector<Colonia> colonias, unordered_map<string, int> colindex) {
    //Matriz de costo y de trayectoria
    vector<vector<int>> mat(m, vector<int>(m));
    vector<vector<int>> p(m, vector<int>(m));

    //Inicializar matrices
    LeerArcos(mat, p, conexiones, m);

    //Algoritmo de floyd
    Floyd(mat, p, n);

    //Hacer una lista de todas las centrales que hay
    vector<Colonia> centrales;
    for (int i = 0; i < colonias.size(); i++) {
        if (colonias[i].esCentral == 1) {
            centrales.push_back(colonias[i]);
        }
    }

    //Hacer las consultas de todas las centrales entre si
    return Consultas(mat, p, centrales, colindex, colonias);
}
