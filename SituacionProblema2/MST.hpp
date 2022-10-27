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

using namespace std;

struct Colonia {
    string nombre;
    pair<double,double> pos;
    bool esCentral;
};

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
	void load(vector<Colonia>, vector<pair<int,int>>);
	void kruskalMST();
	void printEdgesK(vector<Colonia> colonias, string&);
};



//Calcula la distancia de todos los puntos con todos los puntos con dist()
void Graph::load(vector<Colonia> colonias, vector<pair<int,int>> cableadoNuevo){
	for (int i=0; i < colonias.size(); i++){
		for (int j = i+1; j<colonias.size(); j++){
			double d = dist(colonias[i].pos, colonias[j].pos);
			addEdge(i, j, d);
			addEdge(j, i, d);
		}
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