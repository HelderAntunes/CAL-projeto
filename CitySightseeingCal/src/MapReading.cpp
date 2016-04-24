#include "MapReading.h"

/**
 * Calculate the distance between two points
 */
double dist(pair<double,double> p1, pair<double,double> p2){
	return sqrt((p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second));
}

void MapReading::readMap(string nodesFlName, string roadsFlName, string edgesFlName){
	ifstream ifsNodes(nodesFlName.c_str());
	ifstream ifsRoads(roadsFlName.c_str());
	ifstream ifsEdges(edgesFlName.c_str());

	readRoadsFromFile(ifsRoads);
	readNodesFromFile(ifsNodes);
	readEdgesFromFile(ifsEdges);

	ifsNodes.close();
	ifsRoads.close();
	ifsEdges.close();
}

void MapReading::readRoadsFromFile(ifstream& ifs){
	string roadName;
	ll idRoad;
	char pontoVirgula;

	while(1){
		ifs >> idRoad >> pontoVirgula;
		if(ifs.eof())
			break;
		roadName = "";
		char c;
		bool isTwoWay;
		ifs >> c;
		while(c != ';'){
			roadName += c;
			ifs >> c;
		}
		string s;
		ifs >> s;
		if(s == "True")
			isTwoWay = true;
		else
			isTwoWay = false;
		roads[idRoad] = pair<string,bool>(roadName,isTwoWay);
	}
}

void MapReading::readNodesFromFile(ifstream& ifs){
	ll idNodeLong;
	char pontoVirgula;
	double longRad, latRad, lixo;
	int idNode = 0;

	while( 1 ) {
		ifs >> idNodeLong >> pontoVirgula >> lixo >> pontoVirgula >> lixo >> pontoVirgula >> longRad >> pontoVirgula >> latRad;
		if(ifs.eof())
			break;
		mapNodes[idNodeLong] = idNode;
		nodes[idNode] = pair<double,double>(cos(latRad)*cos(longRad)*6371000,cos(latRad)*sin(longRad)*6371000);
		idNode++;
	}
}

void MapReading::readEdgesFromFile(ifstream& ifs){
	char pontoVirgula;
	ll idRoad;
	int idEdge = 0;
	ll origin, dest;

	while( 1 ) {
		ifs >> idRoad >> pontoVirgula >> origin >> pontoVirgula >> dest >> pontoVirgula;
		if(ifs.eof())
			break;
		int o = mapNodes[origin];
		int d = mapNodes[dest];
		mapIdEdgeToIdRoad[idEdge] = idRoad;
		edges[idEdge] = pair<int,int>(o,d);

		double weight = dist(nodes[o], nodes[d]);
		weightOfEdges[idEdge] = pair<double,bool>(weight,roads[idRoad].second);
		idEdge++;
	}
}

void MapReading::sendDataToGraphViewer(GraphViewer *gv){
	double minX = LLONG_MAX;
	double minY = LLONG_MAX;
	double maxX = LLONG_MIN;
	double maxY = LLONG_MIN;

	for(size_t i = 0;i < nodes.size();i++){
		minX = min(minX, nodes[i].first);
		minY = min(minY, nodes[i].second);
		maxX = max(maxX, nodes[i].first);
		maxY = max(maxY, nodes[i].second);
	}
	for(size_t i = 0;i < nodes.size();i++){
		double x, y;
		x = nodes[i].first;
		y = nodes[i].second;
		gv->addNode(i, (x-minX)/(maxX-minX)*850 + 25, (y-minY)/(maxY-minY)*550 + 25);
		gv->setVertexSize(i, 5);
	}
	for(size_t i = 0;i < edges.size();i++){
		int o = edges[i].first;
		int d = edges[i].second;

		if(weightOfEdges[i].second == true)
			gv->addEdge(i, o, d, EdgeType::UNDIRECTED);
		else
			gv->addEdge(i, o, d, EdgeType::DIRECTED);
		gv->setEdgeFlow(i, weightOfEdges[i].first);
	}
}

Graph<int> MapReading::getGraph(){
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

map<int, pair<int,int> > MapReading::getEdges(){
	return edges;
}

