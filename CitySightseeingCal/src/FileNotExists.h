#ifndef SRC_FILENOTEXISTS_H_
#define SRC_FILENOTEXISTS_H_


using namespace std;

class FileNotExists {
private:
	string nameOfFile;
public:
	FileNotExists(string namefl){
		this->nameOfFile = namefl;
	}

	virtual ~FileNotExists(){}

	string getNameOfFile(){
		return nameOfFile;
	}
};

#endif /* SRC_FILENOTEXISTS_H_ */
