#pragma once
/*
Actividad integradora 2
- Patricio Santos Garza A01197723
- Alejandro Hernandez Carrales A01721287
- Rodrigo Galvan Paiz A01721158
*/

#include <algorithm>
#include <climits>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "Colonia.hpp"

using namespace std;

#define INF INT_MAX

//Complejidad: O(N*Q)
vector<string> conexionNueva(vector<Colonia> colonias, vector<Colonia> futurasColonias){
	vector<string> result;
	for (int i = 0; i < futurasColonias.size(); i++){
		int x1 = futurasColonias[i].pos.first;
		int y1 = futurasColonias[i].pos.second;
		int min = INF;
		string cercana;
		for (int j = 0; j < colonias.size(); j++){
			int x2 = colonias[j].pos.first;
			int y2 = colonias[j].pos.second;
			int dist = sqrt(pow(abs(x2-x1), 2) + pow(abs(y2-y1), 2));
			if (dist < min){
				min = dist;
				cercana = colonias[j].nombre;
			}
		}
		result.push_back(futurasColonias[i].nombre + " debe conectarse con " + cercana);
	}
	return result;
	
}