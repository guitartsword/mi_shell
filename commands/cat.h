#pragma once
#include <fstream>

using namespace std;

class cat{
public:
	cat(char*);
	void printFile(); //Uses cout <iostream>
private:
	fstream file;
};
