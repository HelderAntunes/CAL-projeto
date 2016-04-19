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

string getFileName(string fileLetter);
int getOption();
vector<Person> readPersonsFromFile(string fileName);
vector<Person> readPersonsFromInput();
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
	cout << "	Ficheiro A: informacao acerca dos nos\n";
	cout << "	Ficheiro B: informacao acerca das estradas\n";
	cout << "	Ficheiro C: informacao acerca das ligacoes entre os nos\n";
	cout << endl;
	cout << "Prima qualquer tecla para continuar..." << endl;
	cout << endl;
	getchar();

	cout << "Pretende utilizar ficheiros por defeito y/n: ";
	while(1){
		string y;
		getline(cin, y);
		if(y == "y"){
			cout << "Espere um momento...";
			mr.readMap("nodes.txt", "roads.txt", "edges.txt");
			break;
		}
		else if(y == "n"){
			cout << "Espere um momento...";
			mr.readMap(getFileName("A"), getFileName("B"), getFileName("C"));
			break;
		}
		else{
			cout << "Opcao invalida. Tente de novo.\n";
			cout << ">> ";
		}
	}
	cout << endl;
	g = mr.getGraph();

	gv->createWindow(900, 600);
	gv->defineEdgeCurved(false);
	mr.sendDataToGraphViewer(gv);
	gv->rearrange();
	cout << endl;
	cout << "Grafo visivel pelo GraphViewerControler\n";
	cout << endl;

	int idStart = readStartNode();
	int idEnd = readArrivalNode();
	gv->setVertexLabel(idStart, "No de partida");
	gv->setVertexLabel(idEnd, "No de chegada");
	gv->rearrange();

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
	pois = getPoisFromPersons(persons);
	addPoisToGraphViewer(gv,pois);
	gv->rearrange();

	cout << "\n";
	cout << "Indique a capacidade dos autocarros que pretende utilizar.\n";
	cout << "Note que, se o numero de passageiros for superior a capacidade introduzida,\n";
	cout << "irao ser gerados dois caminhos para grupos de turistas com POIs semelhantes.\n";
	capacityBus = readCapacityFromInput();

	cout << endl;
	cout << "Caminho(s) gerado(s)(tambem visiveis pelo GraphViewerController):\n";

	g.floydWarshallShortestPath();

	Graph<int> g2 = Graph<int>();

	// add pois to g2
	pois.insert(idStart);
	pois.insert(idEnd);
	for (std::set<int>::iterator it=pois.begin(); it!=pois.end(); ++it)
		g2.addVertex(*it);

	// add edges to g2
	vector<vector<int> > W = g.getWeightBetweenAllVertexs();
	for (std::set<int>::iterator it=pois.begin(); it!=pois.end(); ++it)
		for (std::set<int>::iterator it2=pois.begin(); it2!=pois.end(); ++it2){
			int weight = W[*it][*it2];
			if(weight != 0 && weight != INT_INFINITY){
				g2.addEdge(*it, *it2, weight);
			}
		}
	vector<int> path = g2.getPathSalesmanProblem(idStart, idEnd);

	cout << "Caminho gerado:\n";
	for(size_t i = 0;i < path.size();i++){
		cout << path[i] << "  ";
	}

	cout << endl;
	double d = 0;
	cout << "Distancia percorrida: " << d << endl;

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
