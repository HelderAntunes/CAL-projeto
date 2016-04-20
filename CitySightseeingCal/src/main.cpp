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
#include <set>
#include "Graph.h"
#include "graphviewer.h"
#include "Person.h"
#include "MapReading.h"

typedef long long int ll;

vector<Person> readPersonsFromFile(string fileName);
int readCapacityFromInput();
int readArrivalNode();
int readStartNode();
void addPoisToGraphViewer(GraphViewer *gv, set<int>& pois);
set<int> getPoisFromPersons(vector<Person>& persons);

int main() {
	GraphViewer *gv = new GraphViewer(900, 600, false);
	MapReading mr;
	set<int> pois;
	Graph<int> g;
	vector<Person> persons;
	int capacityBus;

	cout << "Bem-vindo!\n";

	cout << endl;
	cout << "Para utilizar este programa precisa indicar 3 ficheiros gerados pelo OpenStreetMapsParser.\n";
	cout << "	Ficheiro nodes.txt: informacao acerca dos nos\n";
	cout << "	Ficheiro edges.txt: informacao acerca das estradas\n";
	cout << "	Ficheiro roads.txt: informacao acerca das ligacoes entre os nos\n\n";
	cout << "Precisa tambem do ficheiro\n"
			"	persons.txt: informacao acerca das pessoas e seus pontos de interesse\n"
			"Formatacao deste ficheiro:\n"
			"nome_da_pessoa\n"
			"poi1 poi2 poi3 ... poiN -1\n"
			"...\n\n";
	cout << endl;
	cout << "Prima qualquer tecla para continuar..." << endl;
	cout << endl;
	getchar();

	mr.readMap("nodes.txt", "roads.txt", "edges.txt");
	g = mr.getGraph();

	gv->createWindow(900, 600);
	gv->defineEdgeCurved(false);
	mr.sendDataToGraphViewer(gv);
	gv->rearrange();

	int idStart = readStartNode();
	int idEnd = readArrivalNode();
	gv->setVertexLabel(idStart, "No de partida");
	gv->setVertexLabel(idEnd, "No de chegada");
	gv->rearrange();

	persons = readPersonsFromFile("persons.txt");
	pois = getPoisFromPersons(persons);
	addPoisToGraphViewer(gv,pois);
	gv->rearrange();

	capacityBus = readCapacityFromInput();

	cout << endl;
	cout << "Caminho(s) gerado(s)(tambem visiveis pelo GraphViewerController):\n";
	g.floydWarshallShortestPath();

	Graph<int> g2 = Graph<int>();

	pois.insert(idStart);
	pois.insert(idEnd);
	for (std::set<int>::iterator it=pois.begin(); it!=pois.end(); ++it)
		g2.addVertex(*it);

	vector<vector<int> > W = g.getWeightBetweenAllVertexs();
	for (std::set<int>::iterator it=pois.begin(); it!=pois.end(); ++it)
		for (std::set<int>::iterator it2=pois.begin(); it2!=pois.end(); ++it2){
			int weight = W[*it][*it2];
			if(weight != 0 && weight != INT_INFINITY){
				g2.addEdge(*it, *it2, weight);
			}
		}

	if(g2.isConnected() == false){
		cout << "Alguns pontos de interesse(POIs) sao inacessiveis!!!" << endl;
		return -1;
	}

	vector<int> path = g2.getPathSalesmanProblem(idStart, idEnd);
	double d = 0;

	cout << endl;
	cout << "Caminho gerado:\n";
	cout << path[0] << "  ";
	for(size_t i = 1;i < path.size();i++){
		cout << path[i] << "  " << W[path[i-1]][path[i]] << endl;
		d += W[path[i-1]][path[i]];
	}

	cout << endl;

	cout << "Distancia percorrida: " << d << endl;

	cout << "Pontos de articulacao: ";

	cout << endl;
	cout << "Prima qualquer tecla para terminar..." << endl;
	getchar();

	return 0;
}

set<int> getPoisFromPersons(vector<Person>& persons){
	set<int> pois;
	for(size_t i = 0;i < persons.size();i++){
		vector<int> poisOfPerson = persons[i].getPois();
		for(size_t j = 0;j < poisOfPerson.size();j++)
			pois.insert(poisOfPerson[j]);
	}
	return pois;
}

void addPoisToGraphViewer(GraphViewer *gv, set<int>& pois){
	for (std::set<int>::iterator it=pois.begin(); it!=pois.end(); ++it)
		gv->setVertexColor(*it, "BLUE");
}

int readStartNode(){
	string no;
	int idStart;
	cout << "Indique o no de partida: ";
	while(1){
		getline(cin, no);
		istringstream ss(no);
		if(ss >> idStart)
			return idStart;
		else{
			cout << "No invalido. Tente de novo\n";
			cout << ">> ";
		}
	}
}

int readArrivalNode(){
	string no;
	int idArrival;
	cout << "Indique o no de chegada: ";
	while(1){
		getline(cin, no);
		istringstream ss(no);
		if(ss >> idArrival)
			return idArrival;
		else{
			cout << "No invalido. Tente de novo\n";
			cout << ">> ";
		}
	}
}

int readCapacityFromInput(){
	string s;
	int c;
	while(1){
		cout << "Capacidade dos autocarros: ";
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

