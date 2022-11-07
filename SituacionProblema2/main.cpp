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

int main()
{
    int n, m, k, q, c;
    cin >> n >> m >> k >> q;
    string nombre, col1, col2, writeStr = "-------------------\n1 - Cableado óptimo de nueva conexión.\n";
    double x, y;
    bool esCentral;

    vector<Colonia> colonias(n);
	unordered_map<string, int> colIndex;
	vector<pair<int, pair<int, int>>> conexiones(m);
	vector<pair<int, int>> cableadoNuevo(k);
	vector<Colonia> futurasColonias(q);

    for (int i = 0; i < n; i++){
        cin >> nombre >> x >> y >> esCentral;
        Colonia col;
        col.nombre = nombre;
        col.pos.first = x;
        col.pos.second = y;
        col.esCentral = esCentral;

		colIndex[col.nombre] = i;
        colonias[i] = col;
    }

	for (int i = 0; i < m; i++){
        cin >> col1 >> col2 >> c;		
		conexiones[i] = {c, {colIndex[col1], colIndex[col2]}};
    }

	for (int i = 0; i < k; i++){
        cin >> col1 >> col2;		
		cableadoNuevo[i] = {colIndex[col1], colIndex[col2]};
    }

	for (int i = 0; i < q; i++){
        cin >> nombre >> x >> y;
        Colonia col;
        col.nombre = nombre;
        col.pos.first = x;
        col.pos.second = y;
        col.esCentral = 0;

		colIndex[col.nombre] = i;
        futurasColonias[i] = col;
    }

	//PARTE 1----------------------------------------------------------------

	Graph g(n, n*n);
	g.load(conexiones, cableadoNuevo);
	g.kruskalMST();
	g.printEdgesK(colonias, writeStr);

	//PARTE 2----------------------------------------------------------------

	writeStr += "\n-------------------\n2 - La ruta óptima.\n";

    cout << "Init mat" << endl;
    vector<vector<int>> matAdj(colonias.size()+1, vector<int>(colonias.size()+1, INF));

    iniciaMat(matAdj);
    cout << "leer arcos" << endl;
    leerArcos(matAdj, colonias, conexiones);

    cout << "get tsp" << endl;
    writeStr += "La Ruta Óptima tiene un costo total de: " + to_string(tsp(matAdj, colonias.size(), colonias)) + "\n";
    cout << "tsp fin" << endl;

    //PARTE 2----------------------------------------------------------------
	writeStr += "\n-------------------\n3 - Caminos más cortos entre centrales.\n";

	//cout << "Datos leidos correctamente" << endl;

    //Se escribe todo lo de writeStr en checking.txt
    ofstream outFile("checking.txt");
    outFile << writeStr;
    outFile.close();

    return 0;
}


/*
5 8 1 2
LindaVista 200 120 1
Purisima 150 75 0
Tecnologico -50 20 1
Roma -75 50 0
AltaVista -50 40 0
LindaVista Purisima 48
LindaVista Roma 17
Purisima Tecnologico 40
Purisima Roma 50
Purisima AltaVista 80
Tecnologico Roma 55
Tecnologico AltaVista 15
Roma AltaVista 18
Purisima Tecnologico
Independencia 180 -15
Roble 45 68
*/