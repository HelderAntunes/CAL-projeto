#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <set>
#include "Graph.h"
#include "graphviewer.h"
#include "Person.h"
#include "MapReading.h"

vector<Person> readPersonsFromFile(string fileName);
int readArrivalNode();
int readStartNode();
void addPoisToGraphViewer(GraphViewer *gv, set<int>& pois);
set<int> getPoisFromPersons(vector<Person>& persons);
vector<vector<Person> > agroupPersonsByTheirPois(vector<Person>& persons, vector<set<int> > strongestComponents);
Graph<int> createGraphUsingPois(set<int>& pois, vector<vector<int> >& W);
int calcDistOfPath(vector<int> path, vector<vector<int> >& W);
vector<int> calculatePath(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W, bool& graphIsConnected);
void introduceTheProgram();
void printTourists(vector<Person>& persons);
vector<int> getAllPath(vector<int>& path, Graph<int>& g);
void printPath(vector<int>& path);
vector<int> getArticulationPoints(Graph<int>& g, int idStart);
vector<int> getPointsOfMapThatCanInterruptThePath(vector<int>& path, vector<int>& artPoints);
void printColorEdges(GraphViewer *gv, map<int, pair<int,int> >& edges, map<int, pair<double,bool> >& edgesProperties, vector<int> allPath, int val);
void printStrongestComponents(vector<set<int> > strongestComponents);

int main() {

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

		int distance = calcDistOfPath(path, W);

		cout << "Caminho gerado" << endl;
		vector<int> allPath = getAllPath(path, g);
		printPath(allPath);
		printColorEdges(gv, mr.getEdges(), mr.getEdgesProperties(), allPath, i);
		gv->rearrange();
		cout << endl;

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

	return 0;
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
	for(size_t i = 0;i < path.size();i++)
		cout << path[i] << " ";
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


vector<int> calculatePath(vector<Person>& persons, int idStart, int idEnd, vector<vector<int> >& W, bool& graphIsConnected){
	set<int> pois = getPoisFromPersons(persons);
	pois.insert(idStart);
	pois.insert(idEnd);
	Graph<int> graphWithPois = createGraphUsingPois(pois, W);
	vector<int> path;

	if(graphWithPois.isConnected() == false){
		cout << "Alguns pontos de interesse sao inacessives!!!\n";
		graphIsConnected = false;
		return path;
	}


	path = graphWithPois.getPathSalesmanProblem(idStart, idEnd);
	return path;
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
