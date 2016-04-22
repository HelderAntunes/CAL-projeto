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
void addPoisToGraphViewer(GraphViewer *gv, set<int>& pois);
set<int> getPoisFromPersons(vector<Person>& persons);
vector<vector<Person> > agroupPersonsByTheirPois(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W);
Graph<int> createGraphUsingPois(set<int>& pois, vector<vector<int> >& W);
int calcDistOfPath(vector<int> path, vector<vector<int> >& W);
vector<int> calculatePath(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W);
void introduceTheProgram();

int main() {

	introduceTheProgram();

	GraphViewer *gv = new GraphViewer(900, 600, false);
	MapReading mr ;
	mr.readMap("nodes.txt", "roads.txt", "edges.txt");
	set<int> pois;
	Graph<int> g = mr.getGraph();;
	vector<Person> persons;

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
	pois.insert(idStart);
	pois.insert(idEnd);
	gv->rearrange();

	cout << "Caminho(s) gerado(s)(tambem visiveis pelo GraphViewerController):\n";

	g.floydWarshallShortestPath();
	vector<vector<int> > W = g.getWeightBetweenAllVertexs();

	vector<int> path = calculatePath(persons, idStart, idEnd, W);
	int distance = calcDistOfPath(path, W);
	cout << "Caminho gerado\n";
	for(size_t j = 0;j < path.size();j++){
		cout << path[j];
		if(j < path.size()-1)
			cout << " -> ";
		else
			cout << "\n";
	}
	cout << "Distancia percorrida: " << distance << endl << endl;

	cout << "Pontos de articulacao: ";
	cout << endl;

	cout << "Prima qualquer tecla para terminar..." << endl;
	getchar();

	return 0;
}

void introduceTheProgram(){
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
}

vector<int> calculatePath(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W){
	set<int> pois = getPoisFromPersons(persons);
	pois.insert(idStart);
	pois.insert(idEnd);
	Graph<int> graphWithPois = createGraphUsingPois(pois, W);
	vector<int> path;

	if(graphWithPois.isConnected() == false){
		cout << "Alguns pontos de interesse sao inacessives!!!\n";
		return path;
	}

	path = graphWithPois.getPathSalesmanProblem(idStart, idEnd);
	return path;
}

vector<vector<Person> > agroupPersonsByTheirPois(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W){
	vector<vector<Person> > groups;
	vector<Person> atualGroup;

	atualGroup.push_back(persons[0]);
	groups.push_back(atualGroup);

	for(int i = 1;i < persons.size();i++){
		bool enteredInGroup = false;

		for(int j = 0;j < groups.size();j++){
			atualGroup = groups[j];
			bool isInGroup = true;
			for(int k = 0;k < atualGroup.size();k++){
				if(persons[i].isInSameGroup(atualGroup[k]) == false){
					isInGroup = false;
					break;
				}
			}

			if(isInGroup == true){
				groups[j].push_back(persons[i]);
				enteredInGroup = true;
				break;
			}
		}

		if(enteredInGroup == false){
			vector<Person> newGroup;
			newGroup.push_back(persons[i]);
			groups.push_back(newGroup);
		}
	}

	return groups;
}

int calcDistOfPath(vector<int> path, vector<vector<int> >& W){
	int d = 0;
	for(size_t i = 1;i < path.size();i++){
		int weight = W[path[i]][path[i-1]];
		d += weight;
	}
	return d;
}

Graph<int> createGraphUsingPois(set<int>& pois, vector<vector<int> >& W){
	Graph<int> g;
	vector<int> poisV(pois.begin(), pois.end());
	for(size_t k = 0;k < poisV.size();k++)
		g.addVertex(poisV[k]);
	for(size_t k = 0;k < poisV.size();k++)
		for(size_t w = 0;w < pois.size();w++){
			int weight = W[poisV[k]][poisV[w]];
			if(weight != 0 && weight != INT_INFINITY){
				g.addEdge(poisV[k],poisV[w],weight);
			}
		}
	return g;
}

set<int> getPoisFromPersons(vector<Person>& persons){
	set<int> pois;
	for(size_t i = 0;i < persons.size();i++){
		set<int> poisOfPerson = persons[i].getPois();
		for(set<int>::iterator it = poisOfPerson.begin();it != poisOfPerson.end();it++)
			pois.insert(*it);
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

vector<Person> readPersonsFromFile(string fileName){
	ifstream ifsPersons(fileName.c_str());
	vector<Person> persons;
	string s;
	while(1){
		int poi;
		getline(ifsPersons, s);
		if(ifsPersons.eof() == true)
			break;
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

