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

	bool isInSameGroup(Person p2){
		int count = 0;
		for(set<int>::iterator it = pois.begin();it != pois.end();it++)
			for(set<int>::iterator it2 = p2.pois.begin();it2 != p2.pois.end();it2++)
				if((*it) == (*it2)){
					count++;
					break;
				}

		double perc = (double)count / (double)pois.size();
		if(perc >= 0.5)
			return true;
		else
			return false;
	}

};

#endif /* PERSON_H_ */
