#ifndef SRC_MAPREADING_H_
#define SRC_MAPREADING_H_

#include <map>
#include <iostream>
#include <fstream>
#include <climits>
#include "Graph.h"
#include "graphviewer.h"

using namespace std;

typedef long long int ll;

class MapReading {

private:
	map<ll, int> mapNodes;
	map<int, ll> mapIdEdgeToIdRoad;
	map<ll, pair<string, bool> > roads;
	map<int, pair<double,double> > nodes;
	map<int, pair<int,int> > edges;
	map<int,pair<double,bool> > weightOfEdges;

public:
	MapReading(){};
	virtual ~MapReading(){};

	void readMap(string nodesFlName, string roadsFlName, string edgesFlName);
	void readEdgesFromFile(ifstream& ifs);
	void readNodesFromFile(ifstream& ifs);
	void readRoadsFromFile(ifstream& ifs);

	void sendDataToGraphViewer(GraphViewer *gv);
	Graph<int> getGraph();

};

#endif /* SRC_MAPREADING_H_ */
