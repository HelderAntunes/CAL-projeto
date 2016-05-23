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

	if(ifsNodes.is_open() == false)
		throw FileNotExists("nodes.txt");
	if(ifsRoads.is_open() == false)
		throw FileNotExists("roads.txt");
	if(ifsEdges.is_open() == false)
		throw FileNotExists("edges.txt");

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

map<int, pair<int,int> >& MapReading::getEdges(){
	return edges;
}

map<int, pair<double, bool> >& MapReading::getEdgesProperties(){
	return weightOfEdges;
}

void MapReading::makeManualGraph(GraphViewer *gv){
	int startX = 50, startY = 300, dist = 100;

	gv->createWindow(900, 600);
	gv->defineEdgeCurved(false);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");

	// start node
	gv->addNode(0,startX,startY);

	gv->addNode(1,startX+dist,startY-dist);
	gv->addNode(2,startX+2*dist,startY-2*dist);
	gv->addNode(3,startX+2*dist,startY-50);
	gv->addNode(4,startX+3*dist,startY-dist);
	gv->addNode(5,startX+4*dist,startY-2*dist);
	gv->addNode(6,startX+4*dist,startY-3*dist+50);
	gv->addNode(7,startX+5*dist,startY-2*dist);
	gv->addNode(8,startX+5*dist,startY-dist);
	gv->addNode(9,startX+6*dist,startY-dist);

	gv->addNode(11,startX+dist,startY+dist);
	gv->addNode(12,startX+2*dist,startY+2*dist);
	gv->addNode(13,startX+2*dist,startY+50);
	gv->addNode(14,startX+3*dist,startY+dist);
	gv->addNode(15,startX+4*dist,startY+2*dist);
	gv->addNode(16,startX+4*dist,startY+3*dist-50);
	gv->addNode(17,startX+5*dist,startY+2*dist);
	gv->addNode(18,startX+5*dist,startY+dist);
	gv->addNode(19,startX+6*dist,startY+dist);

	// end node
	gv->addNode(10,startX+7*dist,startY);

	gv->addEdge(0, 0, 1, EdgeType::UNDIRECTED);
	gv->addEdge(1, 1, 2, EdgeType::UNDIRECTED);
	gv->addEdge(2, 2, 4, EdgeType::UNDIRECTED);
	gv->addEdge(3, 3, 4, EdgeType::UNDIRECTED);
	gv->addEdge(4, 1, 3, EdgeType::UNDIRECTED);
	gv->addEdge(5, 4, 5, EdgeType::UNDIRECTED);
	gv->addEdge(6, 5, 6, EdgeType::UNDIRECTED);
	gv->addEdge(7, 6, 7, EdgeType::UNDIRECTED);
	gv->addEdge(8, 5, 7, EdgeType::UNDIRECTED);
	gv->addEdge(9, 7, 8, EdgeType::UNDIRECTED);
	gv->addEdge(10, 8, 9, EdgeType::UNDIRECTED);
	gv->addEdge(11, 9, 10, EdgeType::UNDIRECTED);

	gv->addEdge(12, 0, 11, EdgeType::UNDIRECTED);
	gv->addEdge(13, 11, 13, EdgeType::UNDIRECTED);
	gv->addEdge(14, 13, 14, EdgeType::UNDIRECTED);
	gv->addEdge(15, 14, 12, EdgeType::UNDIRECTED);
	gv->addEdge(16, 11, 12, EdgeType::UNDIRECTED);
	gv->addEdge(17, 14, 15, EdgeType::UNDIRECTED);
	gv->addEdge(18, 15, 16, EdgeType::UNDIRECTED);
	gv->addEdge(19, 16, 17, EdgeType::UNDIRECTED);
	gv->addEdge(20, 16, 18, EdgeType::UNDIRECTED);
	gv->addEdge(21, 17, 18, EdgeType::UNDIRECTED);
	gv->addEdge(22, 18, 19, EdgeType::UNDIRECTED);
	gv->addEdge(23, 19, 10, EdgeType::UNDIRECTED);

	gv->rearrange();
}


