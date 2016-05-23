#ifndef SRC_MAPREADING_H_
#define SRC_MAPREADING_H_

#include <map>
#include <iostream>
#include <fstream>
#include <climits>
#include "Graph.h"
#include "graphviewer.h"
#include "FileNotExists.h"

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
	map<int, string> nameOfNodes;

public:
	MapReading(){};
	virtual ~MapReading(){};

	void readMap(string nodesFlName, string roadsFlName, string edgesFlName);
	void readEdgesFromFile(ifstream& ifs);		//EDGES
	void readNodesFromFile(ifstream& ifs);		//NODES
	void readRoadsFromFile(ifstream& ifs);		//ROADS
	map<int, pair<int,int> >& getEdges();
	map<int, pair<double, bool> >& getEdgesProperties();
	map<int, string>& getNameOfNodes();
	void sendDataToGraphViewer(GraphViewer *gv);
	void sendDataToGraphViewerManual(GraphViewer *gv);
	Graph<int> getGraph();
	void makeManualGraph();

};

#endif /* SRC_MAPREADING_H_ */
