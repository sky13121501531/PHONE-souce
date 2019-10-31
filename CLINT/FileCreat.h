#ifndef __FILECREAT_H_
#define __FILECREAT_H_
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class FileRW
{
public: 
	FileRW();
	~FileRW(){};
public:
	string CreatFile(char* PathFileName);
	string ReadFile(string PathFileName);

};

#endif