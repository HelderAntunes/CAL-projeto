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

	bool isInSameGroup(Person p2, vector<set<int> > strongestComponents){
		int count = 0;
		for(set<int>::iterator it = pois.begin();it != pois.end();it++)
			for(set<int>::iterator it2 = p2.pois.begin();it2 != p2.pois.end();it2++){

				for(size_t i = 0;i < strongestComponents.size();i++){
					set<int> component = strongestComponents[i];
					if(component.find(*it) != component.end()
							&& component.find(*it2) != component.end()){
						count++;
						break;
					}
				}
			}

		double perc = (double)count / (double)(pois.size()*p2.pois.size());
		if(perc >= 0.5)
			return true;
		else
			return false;
	}

};

#endif /* PERSON_H_ */
