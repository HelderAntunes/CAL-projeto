/*
 * Person.h
 *
 *  Created on: 18/04/2016
 *      Author: Asus
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <string>
#include <set>

using namespace std;

class Person {

private:
	string name;
	set<int> pois;

public:
	Person(string name){
		this->name = name;
	}

	virtual ~Person(){};

	string getName(){
		return name;
	}

	set<int> getPois(){
		return pois;
	}

	void addPoi(int Poi){
		this->pois.insert(Poi);
	}

};

#endif /* PERSON_H_ */
