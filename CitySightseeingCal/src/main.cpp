#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <string.h>
#include "Graph.h"
#include "graphviewer.h"
#include "Person.h"
#include "MapReading.h"
#include "Bus.h"

vector<Person> readPersonsFromFile(string fileName);
int readArrivalNode();
int readStartNode();
void addPoisToGraphViewer(GraphViewer *gv, set<int>& pois);
set<int> getPoisFromPersons(vector<Person>& persons);
vector<vector<Person> > agroupPersonsByTheirPois(vector<Person>& persons, vector<set<int> > strongestComponents);
Graph<int> createGraphUsingPois(vector<int>& poisV, vector<vector<int> >& W);
long int calcDistOfPath(vector<int> path, vector<vector<int> >& W);
void introduceTheProgram();
void printTourists(vector<Person>& persons);
vector<int> getAllPath(vector<int>& path, Graph<int>& g);
void printPath(vector<int>& path);
vector<int> getArticulationPoints(Graph<int>& g, int idStart);
vector<int> getPointsOfMapThatCanInterruptThePath(vector<int>& path, vector<int>& artPoints);
void printColorEdges(GraphViewer *gv, map<int, pair<int,int> >& edges, map<int, pair<double,bool> >& edgesProperties, vector<int> allPath, int val);
void printColorVertex(GraphViewer *gv, vector<int>& path);
void printStrongestComponents(vector<set<int> > strongestComponents);
void adicionarTurista(vector<Person> &turistas, map<int, string> nameOfNodes);
void level2(vector<Person> &turistas, map<int, string> nameOfNodes);
void printPOIS( map<int, string> nameOfNodes);
vector<int> calculatePath(vector<int>& pois, vector<vector<int> >& W);
vector<vector<int> > constructPaths(MapReading& mr, GraphViewer *gv);
vector<vector<int> > getPathsFromUser();
vector<int> getPathFromUser(int pathId);
vector<Bus> constructBuses(MapReading& mr, vector<vector<int> >& paths);

void level1(vector<Person> &turistas, map<int, string> nameOfNodes){
	string valor;
	system("cls");

	cout << "1 - Inserir turista" << endl;
	cout << "2 - Exit" << endl;
	cout << "Indique a sua opcao: " ;
	getline(cin, valor);
	int value = atoi(valor.c_str());

	switch (value) {
	case 1:
		adicionarTurista(turistas, nameOfNodes);
		break;
	case 2:
		exit(0);
		break;
	}
}
void adicionarTurista(vector<Person> &turistas, map<int, string> nameOfNodes){
	string nome;
	system("cls");
	cout << "Indique o nome : ";
	getline(cin, nome);
	Person p = Person(nome);
	turistas.push_back(p);
	level2(turistas, nameOfNodes);
}

void chooseByPersons(vector<Person> turistas){
	system("cls");
	string nome;
	printTourists(turistas);
	cout << endl;
	cout << "Escolha o turista com quem quer ir: ";
	getline(cin, nome);

}

void chooseByPOI(map<int, string> nameOfNodes){
	system("cls");
	printPOIS(nameOfNodes);
	cout << "Insira os POIS, por onde pretende passar (Para terminar 0): ";
	string pInteresse = "";
	vector<string> pontosDeInteresse;
	getline(cin, pInteresse);
	pontosDeInteresse.push_back(pInteresse);

}

void level2(vector<Person> &turistas, map<int, string> nameOfNodes){
	system("cls");
	boolean flag;
	string valor = "";

	cout <<"1 - Escolher por pessoas conhecidas" << endl;
	cout <<"2 - Escolher por POIS" << endl;
	cout <<"Indique a sua opcao: ";
	getline(cin,valor);
	int value = atoi(valor.c_str());

	switch (value) {
	case 1:
		if(turistas.size() > 1){
			flag = true;
			chooseByPersons(turistas);
		}
		else{

			system("cls");
		}
		break;
	case 2:
		chooseByPOI(nameOfNodes);
		break;
	default:
		break;
	}


	level1(turistas, nameOfNodes);
}

int main() {
	MapReading mr;
	mr.makeManualGraph();

	GraphViewer *gv = new GraphViewer(900, 600, false);
	gv->createWindow(900, 600);
	gv->defineEdgeCurved(false);
	mr.sendDataToGraphViewerManual(gv);
	gv->rearrange();

	vector<vector<int> > paths = constructPaths(mr, gv);

	mr.sendVertexLabelsToGraphViewer(gv);

	vector<Bus> buses = constructBuses(mr, paths);

	getchar();
	/*
	introduceTheProgram();
	GraphViewer *gv = new GraphViewer(900, 600, false);
	MapReading mr ;
	vector<Person> persons;

	try{
		persons = readPersonsFromFile("persons.txt");
		mr.readMap("nodes.txt", "roads.txt", "edges.txt");
	}
	catch(FileNotExists& fne){
		cout << "Erro ao abrir o ficheiro " << fne.getNameOfFile() << endl;
		cout << "Reinicie o programa e certifique-se de colocar os ficheiros pedidos no lugar correto." << endl;
		return 1;
	}

	set<int> pois;
	Graph<int> g = mr.getGraph();

	gv->createWindow(900, 600);
	gv->defineEdgeCurved(false);
	mr.sendDataToGraphViewer(gv);
	gv->rearrange();

	int idStart = readStartNode();
	int idEnd = readArrivalNode();
	gv->setVertexLabel(idStart, "No de partida");
	gv->setVertexLabel(idEnd, "No de chegada");
	gv->rearrange();

	pois = getPoisFromPersons(persons);
	addPoisToGraphViewer(gv,pois);
	pois.insert(idStart);
	pois.insert(idEnd);
	gv->rearrange();

	cout << "\nCaminho(s) gerado(s):\n\n";

	g.floydWarshallShortestPath();
	vector<vector<int> > W = g.getWeightBetweenAllVertexs();
	vector<set<int> > strongestComponents = g.getStrongestConnectedComponents();
	vector<vector<Person> > groups = agroupPersonsByTheirPois(persons, strongestComponents);
	vector<int> articulationPoints = getArticulationPoints(g, idStart);

	for(size_t i = 0;i < groups.size();i++){
		cout << "Grupo " << i+1 << endl;
		bool poisAreConnected = true;
		vector<int> path = calculatePath(groups[i], idStart, idEnd, W, poisAreConnected);

		if(poisAreConnected == false){
			cout << "Turistas no caminho:" << endl;
			printTourists(groups[i]);
			continue;
		}

		cout << "Caminho gerado" << endl;
		vector<int> allPath = getAllPath(path, g);
		printPath(allPath);
		printColorEdges(gv, mr.getEdges(), mr.getEdgesProperties(), allPath, i);
		gv->rearrange();
		cout << endl;

		long int distance = calcDistOfPath(allPath, W);
		cout << "Distancia percorrida: " << distance << endl << endl;

		vector<int> articulationPointsInPath = getPointsOfMapThatCanInterruptThePath(path, articulationPoints);
		if(articulationPointsInPath.size() > 0){
			cout << "Pontos do caminho que podem interromper a viagem: ";
			for(size_t j = 0;j < articulationPointsInPath.size();j++)
				cout << articulationPointsInPath[j] << "  ";
			cout << endl << endl;
		}

		cout << "Turistas no grupo:" << endl;
		printTourists(groups[i]);
		cout << endl;
	}

	printStrongestComponents(strongestComponents);

	cout << "Prima qualquer tecla para terminar..." << endl;
	getchar();
	 */
	return 0;
}

vector<Bus> constructBuses(MapReading& mr, vector<vector<int> >& paths){
	vector<Bus> buses;
	map<int, string> nameOfNodes = mr.getNameOfNodes();

	for(size_t i = 0;i < paths.size();i++){
		vector<int> path = paths[i];
		Bus bus(path);
		for(size_t j = 0;j < path.size();j++){
			string poiName = nameOfNodes[path[j]];
			bus.addPoi(poiName);
		}
		buses.push_back(bus);
	}

	return buses;
}

vector<vector<int> > constructPaths(MapReading& mr, GraphViewer *gv){
	Graph<int> g = mr.getGraph();
	g.floydWarshallShortestPath();
	vector<vector<int> > W = g.getWeightBetweenAllVertexs();
	vector<vector<int> > paths = getPathsFromUser();

	for(size_t i = 0;i < paths.size();i++){
		vector<int> path = calculatePath(paths[i], W);

		cout << "Caminho " << i+1 << "\n";
		vector<int> allPath = getAllPath(path, g);

		printPath(allPath);
		printColorEdges(gv, mr.getEdges(), mr.getEdgesProperties(), allPath, i);
		printColorVertex(gv, path);
		gv->rearrange();

		paths[i] = allPath;
	}
	return paths;
}

vector<vector<int> > getPathsFromUser(){
	vector<vector<int> > paths;
	int pathId = 0;
	string s;

	cout << "Gerador de autocarros\n";
	while(1){
		if(pathId > 0){
			cout << "Pretende adicionar outro autocarro(y/n)? ";
			getline(cin, s);
			if(s == "n")
				break;
		}
		vector<int> path = getPathFromUser(pathId);
		paths.push_back(path);
		pathId++;
	}
	return paths;
}

vector<int> getPathFromUser(int pathId){
	vector<int> path;
	string s;
	cout << "Indique os Pois do autocarro " << pathId+1 << "\n";

	cout << "Poi de partida: ";
	getline(cin, s);
	path.push_back(atoi(s.c_str()));

	cout << "Poi de chegada: ";
	getline(cin, s);
	path.push_back(atoi(s.c_str()));

	while(1){
		cout << "Indique outro Poi(-1 para terminar): ";
		getline(cin, s);
		if(s == "-1")
			break;
		else
			path.push_back(atoi(s.c_str()));
	}
	return path;
}

void printStrongestComponents(vector<set<int> > strongestComponents){
	cout << "Pontos do mapa com forte conectividade, caso seja preciso alterar a rota :) \n";
	for(size_t i = 0;i < strongestComponents.size();i++){
		set<int> component = strongestComponents[i];
		if(component.size() == 1){
			continue;
		}
		cout << "{";
		int count = 1;
		for(set<int>::iterator j = component.begin();j != component.end();j++){
			if(j == component.begin())
				cout << setw(4) << (*j);
			else
				cout << setw(5) << (*j);
			if(count%11 == 0)
				cout << endl;
			else
				cout << " ";
			count++;
		}
		cout << "}";
		cout << endl;
	}
	cout << endl;
}

vector<int> getAllPath(vector<int>& path, Graph<int>& g){
	vector<int> allPath;
	for(size_t j = 0;j < path.size()-1;j++){
		vector<int> subPath = g.getfloydWarshallPath(path[j], path[j+1]);
		for(size_t k = 0;k < subPath.size();k++){
			if(j > 0 && k == 0)
				continue;
			allPath.push_back(subPath[k]);
		}

	}
	return allPath;
}

vector<int> getPointsOfMapThatCanInterruptThePath(vector<int>& path, vector<int>& artPoints){
	set<int> ansSet;
	for(size_t i = 0;i < path.size();i++)
		for(size_t j = 0;j < artPoints.size();j++)
			if(path[i] == artPoints[j])
				ansSet.insert(path[i]);
	vector<int> ans(ansSet.begin(), ansSet.end());
	return ans;
}

vector<int> getArticulationPoints(Graph<int>& g, int idStart){
	vector<int> artPoints;
	g.findArt(idStart, artPoints);
	for(size_t i = 0;i < artPoints.size();i++)
		if(artPoints[i] == idStart)
			artPoints.erase(artPoints.begin()+i);

	return artPoints;
}

void printPath(vector<int>& path){
	for(size_t i = 0;i < path.size();i++){
		if(i%30 == 0 && i!= 0)
			cout << endl;
		cout << path[i] << " ";
	}
	cout << endl;
}

void printTourists(vector<Person>& persons){
	for(size_t j = 0;j < persons.size();j++)
		cout << persons[j].getName() << endl;
}

void introduceTheProgram(){
	cout << "Bem-vindo!\n";
	cout << endl;
	cout << "Para utilizar este programa precisa colocar 3 ficheiros gerados pelo OpenStreetMapsParser\n";
	cout << "na pasta principal do projeto:\n";
	cout << "	Ficheiro nodes.txt\n";
	cout << "	Ficheiro edges.txt\n";
	cout << "	Ficheiro roads.txt\n";
	cout << "Precisa tambem do ficheiro dos turistas na mesma pasta\n"
			"	persons.txt: informacao acerca das pessoas e seus pontos de interesse\n"
			"Formatacao deste ficheiro:\n"
			"nome_da_pessoa\n"
			"poi1 poi2 poi3 ... poiN -1\n"
			"...\n\n";
	cout << "Prima qualquer tecla para continuar..." << endl;
	cout << endl;
	getchar();
}

vector<int> calculatePath(vector<int>& pois, vector<vector<int> >& W){
	int idStart = pois[0];
	int idEnd = pois[1];
	Graph<int> graphWithPois = createGraphUsingPois(pois, W);
	return graphWithPois.getPathSalesmanProblem(idStart, idEnd);
}

vector<vector<Person> > agroupPersonsByTheirPois(vector<Person>& persons, vector<set<int> > strongestComponents){
	vector<vector<Person> > groups;

	vector<Person> atualGroup;
	atualGroup.push_back(persons[0]);
	groups.push_back(atualGroup);

	for(size_t i = 1;i < persons.size();i++){
		bool enteredInGroup = false;

		for(size_t j = 0;j < groups.size();j++){
			atualGroup = groups[j];
			bool isInGroup = false;

			for(size_t k = 0;k < atualGroup.size();k++){
				if(persons[i].isInSameGroup(atualGroup[k], strongestComponents) == true){
					isInGroup = true;
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

long int calcDistOfPath(vector<int> path, vector<vector<int> >& W){
	long int d = 0;
	for(size_t i = 1;i < path.size();i++){
		d += W[path[i - 1]][path[i]];
	}

	return d;
}

Graph<int> createGraphUsingPois(vector<int>& poisV, vector<vector<int> >& W){
	Graph<int> g;
	for(size_t k = 0;k < poisV.size();k++)
		g.addVertex(poisV[k]);
	for(size_t k = 0;k < poisV.size();k++)
		for(size_t w = 0;w < poisV.size();w++){
			int weight = W[poisV[k]][poisV[w]];
			if(weight != 0 && weight != INT_INFINITY)
				g.addEdge(poisV[k],poisV[w],weight);
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

void printColorEdges(GraphViewer *gv, map<int, pair<int,int> >& edges, map<int, pair<double,bool> >& edgesProperties, vector<int> allPath, int val){

	vector<string> cores;
	cores.push_back("RED");
	cores.push_back("GREEN");
	cores.push_back("ORANGE");
	cores.push_back("PURPLE");
	cores.push_back("BROWN");
	cores.push_back("YELLOW");
	cores.push_back("PINK");

	for (size_t i = 0; i < allPath.size() - 1; ++i)
		for(size_t j = 0; j < edges.size(); j++){
			if(edges[j].first == allPath[i] && edges[j].second == allPath[i + 1])
				gv->setEdgeColor(j, cores[val%7]);
			if(edgesProperties[j].second == true)
				if(edges[j].second == allPath[i] && edges[j].first == allPath[i + 1])
					gv->setEdgeColor(j, cores[val%7]);
		}
}

void printColorVertex(GraphViewer *gv, vector<int>& path){
	for(size_t i = 0;i<  path.size();i++)
		gv->setVertexColor(path[i], "BLUE");
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
	if(ifsPersons.is_open() == false)
		throw FileNotExists(fileName);
	vector<Person> persons;
	string s;
	while(1){
		int poi;
		getline(ifsPersons, s);
		if(ifsPersons.eof() == true || s == "")
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

void printPOIS( map<int, string> nameOfNodes){
	map<int,string>::const_iterator map = nameOfNodes.begin();

	while(map != nameOfNodes.end()){
		if(map->second != "")
			cout << map->second << endl;

		map++;
	}
}
