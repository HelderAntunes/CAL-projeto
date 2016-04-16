#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <climits>
#include <time.h>
#include <algorithm>
#include "Graph.h"

void readMaps();
void createMapManually();

int main() {

	createMapManually();

	getchar();
	return 0;
}

void createMapManually(){
	int hRes = 600, vRes = 600;
	GraphViewer *gv = new GraphViewer(hRes, vRes, false);
	gv->createWindow(hRes, vRes);

	for(int i = 0;i < 4;i++)
		for(int j = 0;j<  4;j++)
			gv->addNode(j+i*4, j*120 + 50, i*120+50);
	srand(time(NULL));

	gv->addEdge(0,0,1, EdgeType::DIRECTED);gv->setEdgeWeight(0, rand()%10);
	gv->addEdge(1,1,2, EdgeType::DIRECTED);gv->setEdgeWeight(1, rand()%10);
	gv->addEdge(2,2,3, EdgeType::DIRECTED);gv->setEdgeWeight(2, rand()%10);
	gv->addEdge(3,7,6, EdgeType::DIRECTED);gv->setEdgeWeight(3, rand()%10);
	gv->addEdge(4,6,5, EdgeType::DIRECTED);gv->setEdgeWeight(4, rand()%10);
	gv->addEdge(5,5,4, EdgeType::DIRECTED);gv->setEdgeWeight(5, rand()%10);
	gv->addEdge(6,8,9, EdgeType::DIRECTED);gv->setEdgeWeight(6, rand()%10);
	gv->addEdge(7,9,10, EdgeType::DIRECTED);gv->setEdgeWeight(7, rand()%10);
	gv->addEdge(8,10,11, EdgeType::DIRECTED);gv->setEdgeWeight(8, rand()%10);
	gv->addEdge(9,15,14, EdgeType::DIRECTED);gv->setEdgeWeight(9, rand()%10);
	gv->addEdge(10,14,13, EdgeType::DIRECTED);gv->setEdgeWeight(10, rand()%10);
	gv->addEdge(11,13,12, EdgeType::DIRECTED);gv->setEdgeWeight(11, rand()%10);

	gv->addEdge(12,0,5, EdgeType::DIRECTED);gv->setEdgeWeight(12, rand()%10);
	gv->addEdge(13,5,10, EdgeType::DIRECTED);gv->setEdgeWeight(13, rand()%10);
	gv->addEdge(14,10,15, EdgeType::DIRECTED);gv->setEdgeWeight(14, rand()%10);

	gv->addEdge(15,0,4, EdgeType::DIRECTED);gv->setEdgeWeight(15, rand()%10);
	gv->addEdge(16,4,8, EdgeType::DIRECTED);gv->setEdgeWeight(16, rand()%10);
	gv->addEdge(17,8,12, EdgeType::DIRECTED);gv->setEdgeWeight(17, rand()%10);
	gv->addEdge(18,3,7, EdgeType::DIRECTED);gv->setEdgeWeight(18, rand()%10);
	gv->addEdge(19,7,11, EdgeType::DIRECTED);gv->setEdgeWeight(19, rand()%10);
	gv->addEdge(20,11,15, EdgeType::DIRECTED);gv->setEdgeWeight(20, rand()%10);
}

/**
 * NOT WORKING!!!
 * Read Maps of street maps site.
 */
void readMaps()
{
	int hRes = 900, vRes = 600;
	GraphViewer *gv = new GraphViewer(hRes, vRes, false);
	gv->createWindow(hRes, vRes);

	ifstream ifs ("nodes.txt");
	long long int idnode;
	char pontoVirgula;
	double x, y, lixo;
	vector< vector<double> > arrNodes;
	double minLat = INT_MAX, maxLat = INT_MIN, minLon = INT_MAX, maxLon = INT_MIN;

	while( !ifs.eof() ) {
		ifs >> idnode >> pontoVirgula >> x >> pontoVirgula >> y >> pontoVirgula >> lixo >> pontoVirgula >> lixo >> pontoVirgula;
		idnode %= 1000000009;
		if(ifs.eof())
			break;
		vector<double> node;
		node.push_back(idnode); node.push_back(x); node.push_back(y);
		minLat = min(x,minLat); maxLat = max(x,maxLat);
		minLon = min(y,minLon); maxLon = max(y, maxLon);
		arrNodes.push_back(node);
	}
	ifs.close();
	cout << arrNodes.size() << "\n";
	for(unsigned int i = 0;i < arrNodes.size();i++){
		int xScreen, yScreen;
		xScreen = (arrNodes[i][1]-minLat)/(maxLat-minLat)*(500)+50;
		yScreen = (arrNodes[i][2]-minLon)/(maxLon-minLon)*(555)+50;
		gv->addNode(arrNodes[i][0], xScreen, yScreen);
		cout << (int)arrNodes[i][0] << endl;
	}
	cout << "fdgfdgfdgdgdf\n" << endl;

	ifstream ifs2 ("edges.txt");
	//srand(time(NULL));
	long long int idedge, ori, dest;
	while( !ifs2.eof() ) {
		ifs2 >> idedge >> pontoVirgula >> ori >> pontoVirgula >> dest >> pontoVirgula;
		if(ifs2.eof())
			break;

		cout << idedge << pontoVirgula << ori << pontoVirgula << dest << pontoVirgula << endl;
		//	gv->addEdge(idedge,ori,dest, EdgeType::DIRECTED);
		//int weight = rand()%50;
		//gv->setEdgeWeight(idedge, weight);
	}
	ifs2.close();
}
