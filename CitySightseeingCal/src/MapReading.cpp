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

void MapReading::sendDataToGraphViewerManual(GraphViewer *gv){

	for(size_t i = 0;i < nodes.size();i++){
		double x, y;
		x = nodes[i].first;
		y = nodes[i].second;
		gv->addNode(i, x, y);
	}

	for(size_t i = 0;i < edges.size();i++){
		int o = edges[i].first;
		int d = edges[i].second;

		if(weightOfEdges[i].second == false)
			gv->addEdge(i, o, d, EdgeType::UNDIRECTED);
		else
			gv->addEdge(i, o, d, EdgeType::DIRECTED);

		gv->setEdgeFlow(i, weightOfEdges[i].first);
	}
}

void MapReading::sendVertexLabelsToGraphViewer(GraphViewer *gv){
	for(size_t i = 0;i < nodes.size();i++){
		gv->setVertexLabel(i, nameOfNodes[i]);
	}
}

void MapReading::makeManualGraph(){
	int startX = 50, startY = 300, dist = 100;

	nodes[0] = pair<double,double>(startX,startY);
	nodes[1] = pair<double,double>(startX+dist,startY-dist);
	nodes[2] = pair<double,double>(startX+2*dist,startY-2*dist);
	nodes[3] = pair<double,double>(startX+2*dist,startY-50);
	nodes[4] = pair<double,double>(startX+3*dist,startY-dist);
	nodes[5] = pair<double,double>(startX+4*dist,startY-2*dist);
	nodes[6] = pair<double,double>(startX+4*dist,startY-3*dist+50);
	nodes[7] = pair<double,double>(startX+5*dist,startY-2*dist);
	nodes[8] = pair<double,double>(startX+5*dist,startY-dist);
	nodes[9] = pair<double,double>(startX+6*dist,startY-dist);
	nodes[11] = pair<double,double>(startX+dist,startY+dist);
	nodes[12] = pair<double,double>(startX+2*dist,startY+2*dist);
	nodes[13] = pair<double,double>(startX+2*dist,startY+50);
	nodes[14] = pair<double,double>(startX+3*dist,startY+dist);
	nodes[15] = pair<double,double>(startX+4*dist,startY+2*dist);
	nodes[16] = pair<double,double>(startX+4*dist,startY+3*dist-50);
	nodes[17] = pair<double,double>(startX+5*dist,startY+2*dist);
	nodes[18] = pair<double,double>(startX+5*dist,startY+dist);
	nodes[19] = pair<double,double>(startX+6*dist,startY+dist);
	nodes[10] = pair<double,double>(startX+7*dist,startY);

	for(int i = 0;i <= 19;i++)
		nameOfNodes[i] = "";
	nameOfNodes[0] = "Infante D Henrique";
	nameOfNodes[1] = "Torre do Clerigos";
	nameOfNodes[4] = "Avenida dos Aliados";
	nameOfNodes[6] = "Praca do Infante";
	nameOfNodes[8] = "Cais de Gaia";
	nameOfNodes[10] = "Alfandega do Porto";
	nameOfNodes[11] = "Palacio da Bolsa";
	nameOfNodes[14] = "Casa da Musica";
	nameOfNodes[16] = "Palacio de Cristal";
	nameOfNodes[18] = "Parque da Cidade";

	edges[0] = pair<int,int>(0,1);
	edges[1] = pair<int,int>(1,2);
	edges[2] = pair<int,int>(2,4);
	edges[3] = pair<int,int>(3,4);
	edges[4] = pair<int,int>(1,3);
	edges[5] = pair<int,int>(4,5);
	edges[6] = pair<int,int>(5,6);
	edges[7] = pair<int,int>(6,7);
	edges[8] = pair<int,int>(5,7);
	edges[9] = pair<int,int>(7,8);
	edges[10] = pair<int,int>(8,9);
	edges[11] = pair<int,int>(9,10);
	edges[12] = pair<int,int>(0,11);
	edges[13] = pair<int,int>(11,13);
	edges[14] = pair<int,int>(13,14);
	edges[15] = pair<int,int>(14,12);
	edges[16] = pair<int,int>(11,12);
	edges[17] = pair<int,int>(14,15);
	edges[18] = pair<int,int>(15,16);
	edges[19] = pair<int,int>(16,17);
	edges[20] = pair<int,int>(16,18);
	edges[21] = pair<int,int>(17,18);
	edges[22] = pair<int,int>(18,19);
	edges[23] = pair<int,int>(19,10);

	weightOfEdges[0] = pair<double,bool>(5,false);
	weightOfEdges[1] = pair<double,bool>(7,false);
	weightOfEdges[2] = pair<double,bool>(4,false);
	weightOfEdges[3] = pair<double,bool>(8,false);
	weightOfEdges[4] = pair<double,bool>(1,false);
	weightOfEdges[5] = pair<double,bool>(4,false);
	weightOfEdges[6] = pair<double,bool>(9,false);
	weightOfEdges[7] = pair<double,bool>(11,false);
	weightOfEdges[8] = pair<double,bool>(35,false);
	weightOfEdges[9] = pair<double,bool>(3,false);
	weightOfEdges[10] = pair<double,bool>(7,false);
	weightOfEdges[11] = pair<double,bool>(3,false);
	weightOfEdges[12] = pair<double,bool>(15,false);
	weightOfEdges[13] = pair<double,bool>(19,false);
	weightOfEdges[14] = pair<double,bool>(7,false);
	weightOfEdges[15] = pair<double,bool>(8,false);
	weightOfEdges[16] = pair<double,bool>(5,false);
	weightOfEdges[17] = pair<double,bool>(9,false);
	weightOfEdges[18] = pair<double,bool>(13,false);
	weightOfEdges[19] = pair<double,bool>(15,false);
	weightOfEdges[20] = pair<double,bool>(18,false);
	weightOfEdges[21] = pair<double,bool>(4,false);
	weightOfEdges[22] = pair<double,bool>(6,false);
	weightOfEdges[23] = pair<double,bool>(18,false);
}

map<int, string> MapReading::getNameOfNodes(){
	return nameOfNodes;
}



