#pragma once
/*
Actividad integradora 2
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <climits>
#include <string>
#include <sstream>
#include <iomanip>

#include "Colonia.hpp"

using namespace std;

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
	int V, E; //Nodos y arcos
	int costMSTKruskal;
	vector<pair<int, pair<int,int>>> edges;
	vector<pair<int, pair<int,int>>> selectedEdgesK;

	Graph(int V, int E){
		this->V = V;
		this->E = E;
		costMSTKruskal = 0;
	}
	// u = salida del arco
	// v = llegada del arco
	// w = costo del arco
	void addEdge(int u, int v, int w){
		edges.push_back({w, {u, v}}); //First = costo, second conexión
	}

	void load(vector<pair<int, pair<int, int>>>, vector<pair<int,int>>);
	void kruskalMST();
	void printEdgesK(vector<Colonia> colonias, string&);
};

//Asigna el costo de los puntos, el costo de los cableados nuevos es 0
void Graph::load(vector<pair<int, pair<int, int>>> conexiones, vector<pair<int,int>> cableadoNuevo){
	for (int i=0; i < conexiones.size(); i++){
		addEdge(conexiones[i].second.first, conexiones[i].second.second, conexiones[i].first);
		addEdge(conexiones[i].second.second, conexiones[i].second.first, conexiones[i].first);
	}

	//Las conexiones de cableado nuevo tendrán prioridad por el costo de 0
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
		if (it.first != 0){ //Se ignoran las conexiones de costo 0
			stringstream stream;
			stream << fixed << setprecision(2) << it.first;
			writestr += colonias[it.second.first].nombre + " - " + colonias[it.second.second].nombre + " " + stream.str() + "\n";
		}	
	}
	
	stringstream stream;
	stream << fixed << setprecision(2) << costMSTKruskal;
	writestr += "\nCosto Total: " + stream.str() + "\n";
}