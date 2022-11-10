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
#include <algorithm>
#include <unordered_map>

#include "S1_MST.hpp"
#include "S2_TSP.hpp"
#include "S3_Floyd.hpp"
#include "S4_ColNueva.hpp"

using namespace std;

int main()
{
    //Se lee la primera línea con las cantidades
    int n, m, k, q, c;
    cin >> n >> m >> k >> q;
    
    string nombre, col1, col2, writeStr;
    int x, y;
    bool esCentral;

    //Vectores para guardar la información de las colonias
    vector<Colonia> colonias(n);
	unordered_map<string, int> colIndex; // Mapa para relacionar el index de las colonias con sus nombres
	vector<pair<int, pair<int, int>>> conexiones(m);
	vector<pair<int, int>> cableadoNuevo(k);
	vector<Colonia> futurasColonias(q);

    //Se leen las colonias, el index es el orden en el que se leen las colonias
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

    //Se leen las conexiones
	for (int i = 0; i < m; i++){
        cin >> col1 >> col2 >> c;		
		conexiones[i] = {c, {colIndex[col1], colIndex[col2]}};
    }

    //Se lee el cableado nuevo
	for (int i = 0; i < k; i++){
        cin >> col1 >> col2;		
		cableadoNuevo[i] = {colIndex[col1], colIndex[col2]};
    }

    //Se leen las colonias nuevas
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
    writeStr = "-------------------\n1 - Cableado óptimo de nueva conexión.\n";

	Graph g(n, n*n);
	g.load(conexiones, cableadoNuevo);
	g.kruskalMST();
	g.printEdgesK(colonias, writeStr);

	//PARTE 2----------------------------------------------------------------
	writeStr += "\n-------------------\n2 - La ruta óptima.\n \n";

    int matAdj[MAX][MAX];
    string matCamino[MAX][MAX];

    //vector<vector<int>> matAdj(colonias.size()+1, vector<int>(colonias.size()+1, INF));
    //vector<vector<string>> matCamino(colonias.size()+1, vector<string>(colonias.size()+1, "#"));

    iniciaMat(matAdj, matCamino);
    leerArcos(matAdj, matCamino, colonias, conexiones);

    string caminoTsp = "";
    string costoTsp = to_string(tsp(matAdj, colonias.size(), colonias, caminoTsp, matCamino));

    writeStr += caminoTsp + "\n \nLa Ruta Óptima tiene un costo total de: " + costoTsp + "\n";

    //PARTE 3 ---------------------------------------------------------------
    writeStr += "\n-------------------\n3 - Caminos mas cortos entre centrales.\n \n";
    
    writeStr += FloydResult(conexiones, n, m, colonias, colIndex);

    //PARTE 4----------------------------------------------------------------
	writeStr += "\n-------------------\n4 - Conexión de nuevas colonias\n\n";

	vector<string> nuevasConexiones = conexionNueva(colonias, futurasColonias);
    
    for (int i = 0; i < nuevasConexiones.size(); i++){
	    writeStr += nuevasConexiones[i] + "\n";
    }
    writeStr += "\n-------------------";

    //Imprimir al archivo checking.txt

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

10 19 0 4
Condesa -193 -151 1
DelValle -142 -70 0
Polanco 144 -122 1
RomaNorte -91 -160 1
Pantitlán -108 42 0
Juárez -97 127 1
Tlatelolco -160 12 0
SantaFe 169 -91 1
Cosmopolita 160 -67 1
Merced 78 16 0
Condesa Polanco 27
Condesa Juárez 68
Condesa Cosmopolita 34
DelValle RomaNorte 80
DelValle Pantitlán 68
DelValle SantaFe 94
DelValle Cosmopolita 86
Polanco Juárez 92
Polanco Tlatelolco 58
Polanco Merced 54
RomaNorte Juárez 69
RomaNorte Cosmopolita 25
Pantitlán Juárez 4
Pantitlán SantaFe 67
Pantitlán Cosmopolita 25
Juárez SantaFe 78
Juárez Cosmopolita 29
Tlatelolco Cosmopolita 82
SantaFe Merced 66
FelipeAngeles -64 25
Tlazintla -85 -106
Penitenciaria -99 -83
Porvenir -95 -196
*/