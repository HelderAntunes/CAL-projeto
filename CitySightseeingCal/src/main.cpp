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

Graph<int> readMaps(ifstream &ifs1, ifstream &ifs2, ifstream &ifs3);
double dist(pair<double,double> p1, pair<double,double> p2);
Graph<int> createGraph(map<int, ll> mapIdEdgeToIdRoad, map<ll, pair<string, bool> > roads, map<int, pair<double,double> > nodes, map<int, pair<int,int> > edges,map<int,pair<double,bool> > weightOfEdges);

int main() {

	Graph<int> g = Graph<int>();

	string s;

	string nodesflName, edgesflName, roadsflName;

	cout << "Bem-vindo!\n";

	cout << endl;
	cout << "Para utilizar este programa precisa indicar 3 ficheiros gerados pelo OpenStreetMapsParser.\n";
	cout << "	Ficheiro A: informacao acerca dos nos\n";
	cout << "	Ficheiro B: informacao acerca das estradas\n";
	cout << "	Ficheiro C: informacao acerca das ligacoes entre os nos\n";
	cout << endl;
	cout << "Prima qualquer tecla para continuar..." << endl;
	getchar();

	ifstream ifs1;
	ifstream ifs2;
	ifstream ifs3;

	while(1){
		cout << "Indique o nome do ficheiro A: ";
		getline(cin, nodesflName);
		ifs1.open(nodesflName.c_str());
		if(ifs1.is_open() == false){
			cout << "Erro ao abrir o ficheiro. Tente de novo.\n";
		}
		else
			break;
	}

	while(1){
		cout << "Indique o nome do ficheiro B: ";
		getline(cin, roadsflName);
		ifs2.open(roadsflName.c_str());
		if(ifs2.is_open() == false)
			cout << "Erro ao abrir o ficheiro. Tente de novo.\n";
		else
			break;
	}

	while(1){
		cout << "Indique o nome do fichero C: ";
		getline(cin, edgesflName);
		ifs3.open(edgesflName.c_str());
		if(ifs3.is_open() == false)
			cout << "Erro ao abrir o ficheiro. Tente de novo.\n";
		else
			break;
	}


	g = readMaps(ifs1, ifs2, ifs3);

	ifs1.close();
	ifs2.close();
	ifs3.close();
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
 */
Graph<int> readMaps(ifstream &ifs1, ifstream &ifs2, ifstream &ifs3){

	GraphViewer *gv = new GraphViewer(900, 600, false);
	gv->createWindow(900, 600);

	map<ll, int> mapNodes;
	map<int, ll> mapIdEdgeToIdRoad;
	map<ll, pair<string, bool> > roads;
	map<int, pair<double,double> > nodes;
	map<int, pair<int,int> > edges;
	map<int,pair<double,bool> > weightOfEdges;

	int avoidProximity_Factor = 2000;
	int idNode = 0;
	int idEdge = 0;
	ll idNodeLong;
	ll idRoad;
	ll origin, dest;
	char pontoVirgula;
	double x, y, lixo;
	double minLat = LLONG_MAX, maxLat = LLONG_MIN;
	double minLon = LLONG_MAX, maxLon = LLONG_MIN;
	string roadName;
	while(!ifs2.eof()){
		ifs2 >> idRoad >> pontoVirgula;
		roadName = "";
		char c;
		bool isTwoWay;
		ifs2 >> c;
		while(c != ';'){
			roadName += c;
			ifs2 >> c;
		}
		string s;
		ifs2 >> s;
		if(s == "True")
			isTwoWay = true;
		else
			isTwoWay = false;
		roads[idRoad] = pair<string,bool>(roadName,isTwoWay);
	}
	while( !ifs1.eof() ) {
		ifs1 >> idNodeLong >> pontoVirgula >> lixo >> pontoVirgula >> lixo >> pontoVirgula >> x >> pontoVirgula >> y;
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
	while( !ifs3.eof() ) {
		ifs3 >> idRoad >> pontoVirgula >> origin >> pontoVirgula >> dest >> pontoVirgula;
		int o = mapNodes[origin];
		int d = mapNodes[dest];
		mapIdEdgeToIdRoad[idEdge] = idRoad;
		edges[idEdge] = pair<int,int>(o,d);

		if(roads[idRoad].second == true){
			gv->addEdge(idEdge, o, d, EdgeType::UNDIRECTED);
		}
		else{
			gv->addEdge(idEdge, o, d, EdgeType::DIRECTED);
		}

		double weight = dist(nodes[o], nodes[d])*avoidProximity_Factor;
		weightOfEdges[idEdge] = pair<double,bool>(weight,roads[idRoad].second);
		gv->setEdgeWeight(idEdge, weight);
		idEdge++;
	}
	return createGraph(mapIdEdgeToIdRoad, roads, nodes, edges, weightOfEdges);
}

Graph<int> createGraph(map<int, ll> mapIdEdgeToIdRoad, map<ll, pair<string, bool> > roads, map<int, pair<double,double> > nodes, map<int, pair<int,int> > edges,map<int,pair<double,bool> > weightOfEdges){
	Graph<int> g = Graph<int>();
	for(unsigned int i = 0;i < nodes.size();i++)
		g.addVertex(i);
	for(unsigned int i = 0;i < edges.size();i++){
		if(weightOfEdges[i].second == true){
			g.addEdge(edges[i].first, edges[i].second, weightOfEdges[i].first);
			g.addEdge(edges[i].second, edges[i].first, weightOfEdges[i].first);
		}
		else{
			g.addEdge(edges[i].first, edges[i].second, weightOfEdges[i].first);
		}

	}
	return g;
}

/**
 * Calculate the distance between two points using haversine formula.
 * Pairs contains latitude and longitude values
 */
double dist(pair<double,double> p1, pair<double,double> p2){
	const double R = 6371000;
	double latRad1 = p1.first;
	double latRad2 = p2.first;
	double lonRad1 = p1.second;
	double lonRad2 = p2.second;
	double difLat = abs(latRad1-latRad2);
	double difLon = abs(lonRad1-lonRad2);

	double a = sin(difLat/2)*sin(difLat/2) +
			cos(latRad1)*cos(latRad2)*
			sin(difLon/2)*sin(difLon/2);
	double c = 2*atan2(sqrt(a), sqrt(1-a));

	return R*c;
}
