#pragma once
/*
Actividad integradora 2
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <iostream>
#include <climits>
#include <queue>
#include <utility>
#include <vector>
#include <string>

#include "Colonia.hpp"

using namespace std;

#define INF INT_MAX
#define MAX 31

struct Nodo{
    int niv; //Nivel del nodo
    int costoAcum; //Costo acumulado
    int costoPos; //Costo posible
    int verticeAnterior;
    int verticeActual;
    vector<bool> visitados;
    bool operator<(const Nodo &otro) const{
        return costoPos >= otro.costoPos;
    }
    string camino;
};

void printMatS(vector<vector<string>> matAdj){
	cout << "----------------" << endl;
	for (int i = 1; i < matAdj.size(); i++){
        cout << i << " ";
		for (int j = 1; j < matAdj[i].size(); j++){
			cout << matAdj[i][j] << "|";
		}
		cout << endl;
	}
}

void printMatI(vector<vector<int>> matAdj){
	cout << "----------------" << endl;
	for (int i = 1; i < matAdj.size(); i++){
		for (int j = 1; j < matAdj[i].size(); j++){
            if (matAdj[i][j] == INF){
                cout << "INF" << " ";
            }
            else {
                cout << matAdj[i][j] << " ";
            }
		}
		cout << endl;
	}
}

//Inicializar las matrices de adjacencias y de camino
void iniciaMat(int matAdj[MAX][MAX], string matCamino[MAX][MAX]){
    for (int i=0; i<MAX; i++){
        matAdj[i][i] = 0;
        matCamino[i][i] = "#";
        for (int j=i+1; j<MAX; j++){
            matAdj[i][j] = matAdj[j][i] = INF;
            matCamino[i][j] = matCamino[j][i] = "#";
        }
    }
}


void leerArcos(int matAdj[MAX][MAX], string matCamino[MAX][MAX], vector<Colonia> colonias, vector<pair<int, pair<int, int>>> conexiones){
    //Se cargan todas las conexiones a matAdj y matCamino
    for (int i = 0; i < conexiones.size(); i++){
        matAdj[conexiones[i].second.first+1][conexiones[i].second.second+1] = 
			matAdj[conexiones[i].second.second+1][conexiones[i].second.first+1] = conexiones[i].first;
		
		matCamino[conexiones[i].second.first+1][conexiones[i].second.second+1] = colonias[conexiones[i].second.second].nombre;
		matCamino[conexiones[i].second.second+1][conexiones[i].second.first+1] = colonias[conexiones[i].second.first].nombre;
    }

    //printMatS(matCamino);
    //Se modifican las tablas para combinar las conexiones de las centrales
    //O(n^3)
	for (int i = 0; i < colonias.size(); i++){
		if (colonias[i].esCentral){
			for (int j = 1; j < colonias.size()+1; j++){
				if (matAdj[i+1][j] != INF){
					for (int k = j+1; k < colonias.size()+1; k++){
						if (matAdj[i+1][k] != INF && matAdj[i+1][j] + matAdj[i+1][k] < matAdj[j][k]){
							matAdj[j][k] = matAdj[k][j] = matAdj[i+1][j] + matAdj[i+1][k];

							matCamino[j][k] = matCamino[j][i+1] + " - " + matCamino[i+1][k];
							matCamino[k][j] = matCamino[k][i+1] + " - " + matCamino[i+1][j];
						}
					}
				}
			}
		}
        //printMatS(matCamino);
	}
}

//Se genera el camino optimo mientras se calcula el costo y regresa el ultimo nodo antes de regresar al incio
int calculaCostoPosible(Nodo &nodoActual, int matAdj[MAX][MAX], string matCamino[MAX][MAX], int n){
    nodoActual.costoPos = nodoActual.costoAcum;
    int costoObtenido, lastNode;
    string nuevoCamino;
    for (int i = 1; i<=n; i++){
        //No se ha visitado o es el ultimo que se visito
        costoObtenido = INF;
        if (!nodoActual.visitados[i] || i == nodoActual.verticeActual){
            if (!nodoActual.visitados[i]){
                for (int j=1; j<=n; j++){
                    if (i!=j && (!nodoActual.visitados[j] || j == 1)){
                        costoObtenido = min(costoObtenido, matAdj[i][j]);
                        if (costoObtenido == matAdj[i][j]){
                            //nuevoCamino = matCamino[i][j];
                        }
                    }
                }
            }
            else{ //El nodo i es el ultimo visitado
                for (int j=1; j<=n; j++){
                    if (!nodoActual.visitados[j]){
                        costoObtenido = min(costoObtenido, matAdj[i][j]);
                        if (costoObtenido == matAdj[i][j]){
                            nuevoCamino = matCamino[i][j];
                            lastNode = j;
                        }
                    }
                }
            }
            if (costoObtenido == INF || nodoActual.costoPos == INF){
                nodoActual.costoPos = INF;
            }
            else {
                nodoActual.costoPos += costoObtenido;
                
            }
        }
    }
    nodoActual.camino += " - " + nuevoCamino;
    return lastNode;
}

//Complejidad: O(2^n)
int tsp(int matAdj[MAX][MAX], int n, vector<Colonia> colonias, string& caminoTsp, string matCamino[MAX][MAX]){
    int costoOptimo = INF;
	int firstNode = 0;
	int centrales = 0;
    int lastNode = 0;
    //El primer nodo es la primer colonia que no sea central
	while(firstNode < colonias.size() && colonias[firstNode].esCentral){
		firstNode++;
		//Si todas las colonias con centrales regresar 0;
		if (firstNode >= colonias.size()){
			return 0;
		}
	}
    Nodo raiz;
    raiz.niv = 0;
    raiz.costoAcum = 0;
    raiz.verticeActual = firstNode+1;
    raiz.verticeAnterior = 0;
	raiz.camino = colonias[firstNode].nombre;
	raiz.visitados = vector<bool>(n+1, false);
    raiz.visitados[firstNode+1] = true;

    //Se ponen todas las centrales como visitadas
	for (int i = 0; i < colonias.size(); i++){
		if (colonias[i].esCentral){
			centrales++;
			raiz.visitados[i+1] = true;
		}
	}
	
    calculaCostoPosible(raiz, matAdj, matCamino, n);
    priority_queue<Nodo> pq;
    pq.push(raiz);
    while (!pq.empty()){
        //Sacar de pq
        Nodo current = pq.top();
        pq.pop();
        //ver si costoPos < costoOptimo
        if (current.costoPos < costoOptimo){
            //Si si, generar posibles hijos
            for (int i = 1; i<=n; i++){
                //Si no se ha visitado el nodo y existe una conexion
                if (!current.visitados[i] && matAdj[i][current.verticeActual] != INF && matAdj[current.verticeActual][i] != INF){
                    //Para cada hijo generar nuevo nodo y actualizar datos
                    Nodo child = current;
                    child.niv = current.niv+1;
                    child.costoAcum = current.costoAcum + matAdj[i][current.verticeActual];
                    child.verticeActual = i;
                    child.verticeAnterior = current.verticeActual;
                    child.visitados[i] = true;
					child.camino = current.camino; //+ " - " + matCamino[child.verticeAnterior][child.verticeActual];
                    int tempLastNode = calculaCostoPosible(child, matAdj, matCamino, n);
                    if (child.niv == n-2-centrales){
                        //si nivel == n-2-centrales, calcular costo real
                        for (int j = 1; j<=n; j++){
                            if (!child.visitados[j]){
                                //Si costo real menor a entonces actualizar costo optimo
                                int costoReal = child.costoAcum + matAdj[child.verticeActual][j] + matAdj[firstNode+1][j];
                                if (costoReal < costoOptimo && costoReal >= 0){
                                    costoOptimo = costoReal;
                                    caminoTsp = child.camino;
                                    lastNode = tempLastNode;
                                }
                            }
                        }
                    }
                    //si n < n-2 .... checar si costo posible es mejor que costo optimo y meter a pq
                    if (child.niv < n-2-centrales && child.costoPos < costoOptimo){
                        pq.push(child);
                    }
                }
            }
        }
    }

	caminoTsp += " - " + matCamino[lastNode][firstNode+1];
    return costoOptimo;
}