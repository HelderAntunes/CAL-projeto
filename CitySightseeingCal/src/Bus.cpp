/*
 * Bus.cpp
 *
 *  Created on: 25/05/2016
 *      Author: Helder Antunes
 */

#include "Bus.h"

void Bus::addTourist(Person tourist){
	tourists.push_back(tourist);
}

void Bus::addPoi(std::string poi){
	pois.append(" ");
	pois.append(poi);
}

std::string Bus::getPois(){
	return pois;
}

std::vector<Person> Bus::getTourists(){
	return tourists;
}


