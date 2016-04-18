/*
 * Person.h
 *
 *  Created on: 18/04/2016
 *      Author: Asus
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <string>

using namespace std;

class Person {

private:
	string name;
	vector<int> pois;

public:
	Person(string name){
		this->name = name;
		this->pois = pois;
	}

	virtual ~Person(){};

	string getName(){
		return name;
	}

	vector<int> getPois(){
		return pois;
	}

	void addPoi(int Poi){
		pois.push_back(Poi);
	}

};

#endif /* PERSON_H_ */
