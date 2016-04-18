#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <climits>
#include <time.h>
#include <map>
#include <string>
#include <algorithm>
#include "Graph.h"
#include "graphviewer.h"
#include "Person.h"

typedef long long int ll;

Graph<int> readMaps(ifstream &ifs1, ifstream &ifs2, ifstream &ifs3);
double dist(pair<double,double> p1, pair<double,double> p2);
Graph<int> createGraph(map<int, ll> mapIdEdgeToIdRoad, map<ll, pair<string, bool> > roads, map<int, pair<double,double> > nodes, map<int, pair<int,int> > edges,map<int,pair<double,bool> > weightOfEdges);
string getFileName(string fileLetter);
int getOption();
vector<Person> readPersonsFromFile(string fileName);
vector<Person> readPersonsFromInput();
int readCapacityFromInput();

int main() {

	Graph<int> g = Graph<int>();
	vector<Person> persons;
	int capacityBus;

	cout << "Bem-vindo!\n";

	cout << endl;
	cout << "Para utilizar este programa precisa indicar 3 ficheiros gerados pelo OpenStreetMapsParser.\n";
	cout << "	Ficheiro A: informacao acerca dos nos\n";
	cout << "	Ficheiro B: informacao acerca das estradas\n";
	cout << "	Ficheiro C: informacao acerca das ligacoes entre os nos\n";
	cout << endl;
	cout << "Prima qualquer tecla para continuar..." << endl;
	getchar();

	cout << "Pretende utilizar ficheiros por defeito y/n: ";

	ifstream ifsNodes;
	ifstream ifsRoads;
	ifstream ifsEdges;
	while(1){
		string y;
		getline(cin, y);
		if(y == "y"){
			ifsNodes.open("nodes.txt");
			ifsRoads.open("roads.txt");
			ifsEdges.open("edges.txt");
			break;
		}
		else if(y == "n"){
			ifsNodes.open(getFileName("A").c_str());
			ifsRoads.open(getFileName("B").c_str());
			ifsEdges.open(getFileName("C").c_str());
			break;
		}
		else{
			cout << "Opcao invalida. Tente de novo.\n";
			cout << ">> ";
		}
	}

	cout << "\nEspere um momento..." << endl;

	g = readMaps(ifsNodes, ifsRoads, ifsEdges);

	ifsNodes.close();
	ifsRoads.close();
	ifsEdges.close();

	cout << endl;
	cout << "Grafo gerado pelo GraphViewerControler\n";
	cout << endl;
	cout << "Prima qualquer tecla para adicionar os turistas e seus pontos de interesse(POIs)" << endl;

	getchar();

	cout << "Para adicionar turistas e seus POIS pode adicionar manualmente ou carrega-los de um ficheiro.\n";
	cout << "Selecione a opcao desejada:\n";
	cout << "1 -> adicionar manualmente\n";
	cout << "2 -> carregar de um ficheiro\n";
	cout << ">> ";
	string s;

	int option = getOption();
	if(option == 1)
		persons = readPersonsFromInput();
	else if(option == 2)
		persons = readPersonsFromFile(getFileName("que contem os turistas e seus POIS"));

	cout << "Indique a capacidade dos autocarros que pretende utilizar.\n";
	cout << "Note que, se o numero de passageiros for superior a capacidade introduzida,\n";
	cout << "irao ser gerados dois caminhos para grupos de turistas com POIs semelhantes.\n";

	capacityBus = readCapacityFromInput();

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

int readCapacityFromInput(){
	string s;
	int c;
	while(1){
		cout << "Capacidade: ";
		getline(cin, s);
		istringstream ss(s);
		if(ss >> c){
			return c;
		}
		else{
			cout << "Capacidade invalida. Tente de novo.\n";
		}
	}
}

vector<Person> readPersonsFromInput(){
	vector<Person> persons;
	string s;
	while(1){
		cout << "Indique o nome de um turista, '0' para acabar a leitura: ";
		getline(cin, s);
		if(s == "0")
			break;
		Person p(s);
		cout << "Pontos de interesse(indique o id do no que representa o POI,\n";
		cout << "visivel pelo GraphViewerController). '-1' indica fim dos POIs.\n";
		while(1){
			cout << "POI: ";
			getline(cin, s);
			if(s == "-1")
				break;
			istringstream ss(s);
			int poi;
			if(ss >> poi)
				p.addPoi(poi);
			else
				cout << "Poi invalido. Tente de novo.\n";
		}
		persons.push_back(p);
	}
	cout << endl;
	return persons;
}

vector<Person> readPersonsFromFile(string fileName){
	ifstream ifsPersons(fileName.c_str());
	vector<Person> persons;
	string s;
	while(!ifsPersons.eof()){
		int poi;
		getline(ifsPersons, s);
		Person p(s);
		while(1){
			ifsPersons >> poi;
			if(poi == -1)
				break;
			p.addPoi(poi);
		}
		ifsPersons.ignore();
		persons.push_back(p);
	}
	ifsPersons.close();
	return persons;
}

/**
 * FileLetter can be 'A', 'B' or 'C'
 */
string getFileName(string fileLetter){
	ifstream ifs;
	string fileName;
	while(1){
		cout << "Indique o nome do ficheiro " << fileLetter << ": ";
		getline(cin, fileName);
		ifs.open(fileName.c_str());
		if(ifs.is_open() == false){
			cout << "Erro ao abrir o ficheiro. Tente de novo.\n";
		}
		else
			break;
	}
	ifs.close();
	return fileName;
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
	gv->defineEdgeCurved(false);

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
	double longRad, latRad, lixo;
	double minX = LLONG_MAX;
	double minY = LLONG_MAX;
	double maxX = LLONG_MIN;
	double maxY = LLONG_MIN;
	string roadName;
	while(1){
		ifs2 >> idRoad >> pontoVirgula;
		if(ifs2.eof())
			break;
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
	while( 1 ) {
		ifs1 >> idNodeLong >> pontoVirgula >> lixo >> pontoVirgula >> lixo >> pontoVirgula >> longRad >> pontoVirgula >> latRad;
		if(ifs1.eof())
			break;
		mapNodes[idNodeLong] = idNode;
		nodes[idNode] = pair<double,double>(longRad,latRad);
		minX = min(minX, (cos(latRad)*cos(longRad)*6371000));
		minY = min(minY, (cos(latRad)*sin(longRad)*6371000));
		maxX = max(maxX, (cos(latRad)*cos(longRad)*6371000));
		maxY = max(maxY, (cos(latRad)*sin(longRad)*6371000));
		idNode++;
	}

	for(unsigned int i = 0;i < nodes.size();i++){
		double x, y;
		longRad = nodes[i].first;
		latRad = nodes[i].second;
		x = (cos(latRad)*cos(longRad)*6371000);
		y = (cos(latRad)*sin(longRad)*6371000);
		gv->addNode(i, (x-minX)/(maxX-minX)*850 + 25, (y-minY)/(maxY-minY)*550 + 25);
		gv->setVertexSize(i, 5);
	}
	while( 1 ) {
		ifs3 >> idRoad >> pontoVirgula >> origin >> pontoVirgula >> dest >> pontoVirgula;
		if(ifs3.eof())
			break;
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
		//gv->setEdgeLabel(idEdge, roads[idRoad].first);
		idEdge++;
	}
	return createGraph(mapIdEdgeToIdRoad, roads, nodes, edges, weightOfEdges);
}

int getOption(){
	string s;
	while(1){
		getline(cin, s);
		if(s == "1")
			return 1;
		else if(s == "2")
			return 2;
		else{
			cout << "Opcao invalida. Tente de novo.\n";
			cout << ">> ";
		}
	}
}

Graph<int> createGraph(map<int, ll> mapIdEdgeToIdRoad, map<ll, pair<string, bool> > roads, map<int, pair<double,double> > nodes, map<int, pair<int,int> > edges,map<int,pair<double,bool> > weightOfEdges){
	Graph<int> g = Graph<int>();
	for(unsigned int i = 0;i < nodes.size();i++)
		g.addVertex(i);
	for(unsigned int i = 0;i < edges.size();i++)
		if(weightOfEdges[i].second == true){
			g.addEdge(edges[i].first, edges[i].second, weightOfEdges[i].first);
			g.addEdge(edges[i].second, edges[i].first, weightOfEdges[i].first);
		}
		else
			g.addEdge(edges[i].first, edges[i].second, weightOfEdges[i].first);
	return g;
}

/**
 * Calculate the distance between two points using 'haversine' formula.
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
