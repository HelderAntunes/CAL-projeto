/*
 * Bus.h
 *
 *  Created on: 25/05/2016
 *      Author: Helder Antunes
 */

#ifndef SRC_BUS_H_
#define SRC_BUS_H_

#include <string>
#include <vector>
#include "Person.h"

class Bus {

private:
	std::vector<int> path;
	std::string pois;
	std::vector<Person> tourists;

public:
	Bus(std::vector<int> path){this->path = path;};
	virtual ~Bus(){};
	void addTourist(Person tourist);
	void addPoi(std::string poi);
	std::string getPois();
	std::vector<Person> getTourists();
};

#endif /* SRC_BUS_H_ */
