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
int readArrivalNode();
int readStartNode();
void addPoisToGraphViewer(GraphViewer *gv, vector<int>& pois);
vector<int> getPoisFromPersons(vector<Person>& persons);
vector<Graph<int> > calculatePaths(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W);
Graph<int> createGraphUsingPois(vector<int>& pois, vector<vector<int> >& W);
int calcDistOfPath(vector<int> path, vector<vector<int> >& W);

int main() {
	GraphViewer *gv = new GraphViewer(900, 600, false);
	MapReading mr;
	vector<int> pois;
	Graph<int> g;
	vector<Person> persons;

	cout << "Bem-vindo!\n";

	cout << endl;
	cout << "Para utilizar este programa precisa indicar 3 ficheiros gerados pelo OpenStreetMapsParser.\n";
	cout << "	Ficheiro nodes.txt\n";
	cout << "	Ficheiro edges.txt\n";
	cout << "	Ficheiro roads.txt\n";
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

	cout << endl;
	cout << "Caminho(s) gerado(s)(tambem visiveis pelo GraphViewerController):\n";

	g.floydWarshallShortestPath();
	vector<vector<int> > W = g.getWeightBetweenAllVertexs();

	Graph<int> graphToTestConnectivity = createGraphUsingPois(pois, W);
	if(graphToTestConnectivity.isConnected() == false){
		cout << "Alguns pontos de interesse sao inacessiveis!!!" << endl;
		cout << "Certifique-se que todos os POIs, no de partida e destino sao conexos entre si..." << endl;
		return -1;
	}

	vector<Graph<int> > graphs = calculatePaths(persons, idStart, idEnd, W);

	cout << "Pontos de articulacao: ";

	cout << endl;
	cout << "Prima qualquer tecla para terminar..." << endl;
	getchar();

	return 0;
}

vector<Graph<int> > calculatePaths(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W){
	vector<Graph<int> > graphs;
	for(size_t i = 0;i < persons.size();i++){
		Person p = persons[i];
		vector<int> pPois = p.getPois();
		pPois.push_back(idStart);
		pPois.push_back(idEnd);
		Graph<int> gp = createGraphUsingPois(pPois, W);

		int factorAditive;
		int graphToAdd = -1;
		vector<int> vertixesToAdd;

		vector<int> pathP = gp.getPathSalesmanProblem(idStart, idEnd);
		int distanceP = calcDistOfPath(pathP, W);
		factorAditive = distanceP;

		for(size_t j = 0;j < graphs.size();j++){

			Graph<int> gCalculated = graphs[j];
			vector<int> pathOfGC = gCalculated.getPathSalesmanProblem(idStart, idEnd);
			int distOriginal =calcDistOfPath(pathOfGC, W);
			vector<Vertex<int> * > vp = gCalculated.getVertexSet();
			vector<int> totalVertixes;
			for(size_t k = 0;k < vp.size();k++){
				totalVertixes.push_back(vp[k]->getInfo());
			}
			for(size_t k = 0;k < pPois.size();k++)
				totalVertixes.push_back(pPois[k]);
			Graph<int> newGraph = createGraphUsingPois(totalVertixes, W);

			if(newGraph.isConnected() == false)
				continue;

			vector<int> pathWithAdd = gCalculated.getPathSalesmanProblem(idStart, idEnd);
			int distWithAdd = calcDistOfPath(pathWithAdd, W);

			if(distWithAdd-distOriginal < factorAditive){
				factorAditive = distWithAdd-distOriginal;
				graphToAdd = j;
				vertixesToAdd = totalVertixes;
			}
		}

		if(graphToAdd == -1){
			graphs.push_back(gp);
		}
		else{
			graphs.erase(graphs.begin()+graphToAdd);
			Graph<int> newGraph = createGraphUsingPois(vertixesToAdd, W);
			newGraph.getPathSalesmanProblem(idStart,idEnd);
			graphs.push_back(newGraph);
		}
	}
	return graphs;
}

int calcDistOfPath(vector<int> path, vector<vector<int> >& W){
	int d = 0;
	for(size_t i = 0;i < path.size();i++){
		int weight = W[path[i]][path[i-1]];
		d += weight;
	}
	return d;
}

Graph<int> createGraphUsingPois(vector<int>& pois, vector<vector<int> >& W){
	Graph<int> g;
	for(size_t k = 0;k < pois.size();k++)
		g.addVertex(pois[k]);
	for(size_t k = 0;k < pois.size();k++){
		for(size_t w = 0;w < pois.size();w++){
			int weight = W[k][w];
			if(weight != 0 && weight != INT_INFINITY){
				g.addEdge(k,w,weight);
			}
		}
	}
	return g;
}

vector<int> getPoisFromPersons(vector<Person>& persons){
	vector<int> pois;
	for(size_t i = 0;i < persons.size();i++){
		vector<int> poisOfPerson = persons[i].getPois();
		for(size_t j = 0;j < poisOfPerson.size();j++)
			pois.push_back(poisOfPerson[j]);
	}
	return pois;
}

void addPoisToGraphViewer(GraphViewer *gv, vector<int>& pois){
	for (std::vector<int>::iterator it=pois.begin(); it!=pois.end(); ++it)
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

