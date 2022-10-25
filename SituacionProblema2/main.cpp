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

struct Graph {
	// V = Cantidad de nodos (Vertex)
	// E = Cantidaf de arcos (Edges)
	int V, E;
	double costMSTKruskal;
	vector<pair<double, pair<int,int>>> edges; 	// Utilizar en Krukal
	vector<pair<int,int>> selectedEdgesK;		// Los arcos sel Kruskal
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
	void load(vector<pair<double,double>>);
	void kruskalMST();
	void printEdgesK();
};

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

//Calcula la distancia de todos los puntos con todos los puntos con dist()
void Graph::load(vector<pair<double,double>> coords){
	for (int i=0; i<coords.size(); i++){
		for (int j = i+1; j<coords.size(); j++){
			double d = dist(coords[i], coords[j]);
			addEdge(i, j, d);
			addEdge(j, i, d);
		}
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
			selectedEdgesK.push_back({it.second.first,it.second.second});
			ds.merge(it.second.first,it.second.second);
		}
	}
}

void Graph::printEdgesK(){
	cout << "Selected Edges Kruskal: ";
	for (auto it:selectedEdgesK){
		cout << "(" << it.first << "," << it.second << ") ";
	}
	cout << endl;
}


int main()
{
    int n, m, k, q;
    cin >> n >> m >> k >> q;
    string nombre, writeStr = "";
    double x, y;
    bool esCentral;

    vector<Colonia> colonias(n);

    for (int i = 0; i < n; i++){
        cin >> nombre >> x >> y >> esCentral;
        Colonia col;
        col.nombre = nombre;
        col.pos.first = x;
        col.pos.second = y;
        col.esCentral = esCentral;
        colonias[i] = col;
    }

    //Se escribe todo lo de writeStr en checking.txt
    ofstream outFile("checking.txt");
    outFile << writeStr;
    outFile.close();

    return 0;
}