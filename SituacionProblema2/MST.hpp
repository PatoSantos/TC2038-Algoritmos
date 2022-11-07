#pragma once
/*
Actividad integradora 2
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <algorithm>
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// CLASES DE LA APLICACION ==========================================================================
#define INF INT_MAX
#define MAX 21 //Max para TSP (Seccion 2)

struct Colonia {
    string nombre;
    pair<double,double> pos;
    bool esCentral;
};

// FUNCIONES DE LA SECCION 1 ==========================================================================
//Calcula la distancia entre las coordenadas
double dist(pair<double,double> a, pair<double,double> b){
	return sqrt(pow(b.first - a.first, 2) + pow(b.second - a.second, 2) * 1.0);
}

// Disjont Sets (Union-Find)
struct DisjointSets{
	int *parent, *rnk;
	int n;
	DisjointSets(int n){
		this->n = n;
		parent = new int[n+1];
		rnk = new int[n+1];
		for (int i=0; i<=n; i++){
			rnk[i] = 0;
			parent[i] = i;
		}
	}
	// Para encontrar el parent de 'u'
	int find(int u){
		if (u != parent[u]){
			parent[u] = find(parent[u]);
		}
		return parent[u];
	}

	// Union por rank
	void merge(int x, int y){
		x = find(x);
		y = find(y);
		if (rnk[x] > rnk[y]){
			parent[y] = x;
		}
		else{
			parent[x] = y;
		}
		if (rnk[x] == rnk[y]){
			rnk[y]++;
		}
	}
};

struct Graph {
	// V = Cantidad de nodos (Vertex)
	// E = Cantidaf de arcos (Edges)
	int V, E;
	double costMSTKruskal;
	vector<pair<double, pair<int,int>>> edges; 	// Utilizar en Krukal
	vector<pair<double, pair<int,int>>> selectedEdgesK;		// Los arcos sel Kruskal
	Graph(int V, int E){
		this->V = V;
		this->E = E;
		costMSTKruskal = 0;
	}
	// u = salida del arco
	// v = llegada del arco
	// w = costo del arco
	void addEdge(int u, int v, double w){
		edges.push_back({w, {u, v}}); //First = costo, second conexión
	}
	void load(vector<pair<int, pair<int, int>>>, vector<pair<int,int>>);
	void kruskalMST();
	void printEdgesK(vector<Colonia> colonias, string&);
};

//Calcula la distancia de todos los puntos con todos los puntos con dist()
void Graph::load(vector<pair<int, pair<int, int>>> conexiones, vector<pair<int,int>> cableadoNuevo){
	for (int i=0; i < conexiones.size(); i++){
		addEdge(conexiones[i].second.first, conexiones[i].second.second, conexiones[i].first);
		addEdge(conexiones[i].second.second, conexiones[i].second.first, conexiones[i].first);
	}
	for (int i=0; i < cableadoNuevo.size(); i++){
		addEdge(cableadoNuevo[i].first, cableadoNuevo[i].second, 0);
		addEdge(cableadoNuevo[i].second, cableadoNuevo[i].first, 0);
	}
}

// Complejidad: P(E logE)
void Graph::kruskalMST(){
	sort(edges.begin(), edges.end());
	DisjointSets ds(V);
	costMSTKruskal = 0;
	for (auto it:edges){
		int p1 = ds.find(it.second.first);
		int p2 = ds.find(it.second.second);

		if (p1 != p2){
			costMSTKruskal += it.first;
			selectedEdgesK.push_back({it.first,{it.second.first,it.second.second}});
			ds.merge(it.second.first,it.second.second);
		}
	}
}

void Graph::printEdgesK(vector<Colonia> colonias, string &writestr){
	writestr += "\n";
	for (auto it:selectedEdgesK){
		stringstream stream;
		stream << fixed << setprecision(2) << it.first;
		writestr += colonias[it.second.first].nombre + " - " + colonias[it.second.second].nombre + " " + stream.str() + "\n";
	}
	
	stringstream stream;
	stream << fixed << setprecision(2) << costMSTKruskal;
	writestr += "\nCosto Total: " + stream.str() + "\n";
}

// FUNCIONES DE LA SECCION 2 ==========================================================================

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
};

void iniciaMat(vector<vector<int>>& matAdj){
    for (int i=0; i<matAdj.size(); i++){
        matAdj[i][i] = 0;
    }
}

void printMat(vector<vector<int>> matAdj){
	cout << "----------------" << endl;
	for (int i = 1; i < matAdj.size(); i++){
		for (int j = 1; j < matAdj[i].size(); j++){
			if (matAdj[i][j] == INF) {
				cout << "IN" << " ";
			} else {
				cout << matAdj[i][j] << " ";
			}
		}
		cout << endl;
	}
}

void leerArcos(vector<vector<int>>& matAdj, vector<Colonia> colonias, vector<pair<int, pair<int, int>>> conexiones){
    for (int i = 0; i < conexiones.size(); i++){
        matAdj[conexiones[i].second.first+1][conexiones[i].second.second+1] = 
			matAdj[conexiones[i].second.second+1][conexiones[i].second.first+1] = conexiones[i].first;
    }
	for (int i = 0; i < colonias.size(); i++){
		if (colonias[i].esCentral){
			for (int j = 1; j < matAdj[i+1].size(); j++){
				if (matAdj[i+1][j] != INF){
					for (int k = j+1; k < matAdj[i+1].size(); k++){
						if (matAdj[i+1][k] != INF && matAdj[i+1][j] + matAdj[i+1][k] < matAdj[j][k]){
							matAdj[j][k] = matAdj[k][j] = matAdj[i+1][j] + matAdj[i+1][k];
						}
					}
				}
			}
		}
	}
}

void calculaCostoPosible(Nodo &nodoActual, vector<vector<int>>& matAdj, int n){
    nodoActual.costoPos = nodoActual.costoAcum;
    int costoObtenido;
    for (int i = 1; i<=n; i++){
        //No se ha visitado o es el ultimo que se visito
        costoObtenido = INF;
        if (!nodoActual.visitados[i] || i == nodoActual.verticeActual){
            if (!nodoActual.visitados[i]){
                for (int j=1; j<=n; j++){
                    if (i!=j && (!nodoActual.visitados[j] || j == 1)){
                        costoObtenido = min(costoObtenido, matAdj[i][j]);
                    }
                }
            }
            else{ //El nodo i es el ultimo visitado
                for (int j=1; j<=n; j++){
                    if (!nodoActual.visitados[j]){
                        costoObtenido = min(costoObtenido, matAdj[i][j]);
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
}

//Complejidad: O(2^n)
int tsp(vector<vector<int>>& matAdj, int n, vector<Colonia> colonias){
    int costoOptimo = INF;
	int firstNode = 0;
	int centrales = 0;
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
	cout << "Cancelar centrales" << endl;
	raiz.visitados = vector<bool>(matAdj.size()+1, false);
    raiz.visitados[firstNode+1] = true;
	for (int i = 0; i < colonias.size(); i++){
		if (colonias[i].esCentral){
			centrales++;
			raiz.visitados[i+1] = true;
		}
	}
	cout << "centrales canceladas" << endl;
    calculaCostoPosible(raiz, matAdj, n);
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
                    calculaCostoPosible(child, matAdj, n);
                    if (child.niv == n-2-centrales){
                        //si nivel == n-2, calcular costo real
                        for (int j = 1; j<=n; j++){
                            if (!child.visitados[j]){
                                //Si costo real menor a entonces actualizar costo optimo
                                int costoReal = child.costoAcum + matAdj[child.verticeActual][j] + matAdj[firstNode+1][j];
                                if (costoReal < costoOptimo && costoReal >= 0){
                                    costoOptimo = costoReal;
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
    return costoOptimo;
}

//Complejidad: O(n^2)
vector<string> conexionNueva(vector<Colonia> colonias, vector<Colonia> futuras){
	vector<string> result;
	for (int i = 0; i < futuras.size(); i++){
		int x1 = futuras[i].pos.first;
		int y1 = futuras[i].pos.second;
		double min = INF;
		string cercana;
		for (int j = 0; j < colonias.size(); j++){
			int x2 = colonias[j].pos.first;
			int y2 = colonias[j].pos.second;
			double dist = sqrt(pow(abs(x2-x1), 2) + pow(abs(y2-y1), 2));
			if (dist < min){
				min = dist;
				cercana = colonias[j].nombre;
			}
		}
		result.push_back(futuras[i].nombre + " debe conectarse con " + cercana);
	}
	return result;
	
}