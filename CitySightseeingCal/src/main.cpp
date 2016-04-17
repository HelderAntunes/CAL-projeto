#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <climits>
#include <time.h>
#include <map>
#include <algorithm>
#include "Graph.h"
#include "graphviewer.h"

typedef long long int ll;

void readMaps();
double dist(pair<double,double> p1, pair<double,double> p2);

int main() {
	readMaps();

	getchar();

	string s;

	cout << "Bem-vindo!\n";

	cout << endl;
	cout << "Para utilizar este programa precisa indicar 3 ficheiros gerados pelo OpenStreetMapsParser.\n";
	cout << "	Ficheiro A: informacao acerca dos nos\n";
	cout << "	Ficheiro B: informacao acerca das estradas\n";
	cout << "	Ficheiro C: informacao acerca das ligacoes entre os nos\n";
	cout << endl;
	cout << "Prima qualquer tecla para continuar..." << endl;
	getchar();

	cout << "Indique o nome do ficheiro A: ";
	getline(cin, s);
	cout << "Indique o nome do ficheiro B: ";
	getline(cin, s);
	cout << "Indique o nome do fichero C: ";
	getline(cin, s);

	cout << endl;
	cout << "Grafo gerado pelo GraphViewerControler\n";
	cout << endl;
	cout << "Prima qualquer tecla para adicionar os turistas e seus pontos de interesse(POIs)" << endl;

	getchar();

	while(1){
		cout << "Indique o nome de um turista, '0' para acabar a leitura: ";
		getline(cin, s);
		if(s == "0")
			break;
		cout << "Pontos de interesse(indique o id do no que representa o POI,\n";
		cout << "visivel pelo GraphViewerController). '-1' indica fim dos POIs.\n";

		while(1){
			cout << "POI: ";
			getline(cin, s);
			if(s == "-1")
				break;
		}
	}
	cout << endl;

	cout << "Indique a capacidade dos autocarros que pretende utilizar.\n";
	cout << "Note que, se o numero de passageiros for superior a capcidade introduzida,\n";
	cout << "irao ser gerados dois caminhos para grupos de turistas com POIs semelhantes.\n";
	cout << "Capacidade: ";
	getline(cin, s);

	cout << endl;
	cout << "Caminho(s) gerado(s)(tambem visiveis pelo GraphViewerController):\n";
	// gerar caminhos do tipo n1->n2->...->nn
	// no GraphViewerController podemos ver os caminhos coloridos :)
	double d = 0;
	cout << "Distancia percorrida: " << d << endl;

	cout << endl;
	cout << "Prima qualquer tecla para terminar..." << endl;
	getchar();

	return 0;
}

/**
 * Read maps generated from OpenStreetMapsParser.
 *
 * Some values in file can exceed int gamma values. So
 * It are used mapNodes and mapEdges to map long long int(ll)
 * to int.
 *
 * NOT implemented yet:
 * -> calculate distance between two points using latitude and longitude.
 * -> read name of roads, and see if a road is two way.
 * -> add to Graph class.
 *
 * For now, is possible see the graph:)
 */
void readMaps()
{
	GraphViewer *gv = new GraphViewer(900, 600, false);
	gv->createWindow(900, 600);

	ifstream ifs1 ("nodes.txt");
	ifstream ifs2 ("edges.txt");
	map<ll, int> mapNodes;
	map<ll, int> mapEdges;
	map<int, pair<double,double> > nodes;
	map<int, pair<double,double> > edges;
	int avoidProximity_Factor = 2000;
	int idNode = 0;
	int idEdge = 0;
	ll idNodeLong;
	ll idEdgeLong;
	ll origin, dest;
	char pontoVirgula;
	double x, y, lixo;
	double minLat = LLONG_MAX, maxLat = LLONG_MIN;
	double minLon = LLONG_MAX, maxLon = LLONG_MIN;

	while( !ifs1.eof() ) {
		ifs1 >> idNodeLong >> pontoVirgula >> x >> pontoVirgula >> y >>
		pontoVirgula >> lixo >> pontoVirgula >> lixo;
		mapNodes[idNodeLong] = idNode;
		nodes[idNode] = pair<double,double>(x,y);
		minLat = min(x,minLat); maxLat = max(x,maxLat);
		minLon = min(y,minLon); maxLon = max(y, maxLon);
		idNode++;
	}

	for(unsigned int i = 0;i < nodes.size();i++){
		int xScreen, yScreen;
		x = nodes[i].first;
		y = nodes[i].second;
		xScreen = (x-minLat)/(maxLat-minLat)*avoidProximity_Factor+50;
		yScreen = (y-minLon)/(maxLon-minLon)*avoidProximity_Factor+50;
		gv->addNode(i, xScreen, yScreen);
	}

	while( !ifs2.eof() ) {
		ifs2 >> idEdgeLong >> pontoVirgula >> origin >> pontoVirgula
		>> dest >> pontoVirgula;
		mapEdges[idEdgeLong] = idEdge;
		int o = mapNodes[origin];
		int d = mapNodes[dest];
		gv->addEdge(idEdge, o, d, EdgeType::DIRECTED);
		int weight = dist(nodes[o], nodes[d])*avoidProximity_Factor;
		gv->setEdgeWeight(idEdge, weight);
		idEdge++;
	}
	ifs1.close();
	ifs2.close();
}

/**
 * Calculate the distance between two points
 */
double dist(pair<double,double> p1, pair<double,double> p2){
	return sqrt(  (p1.first-p2.first)*(p1.first-p2.first) + (p1.second-p2.second)*(p1.second-p2.second));
}
