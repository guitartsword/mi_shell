#include "cat.h"
#include <fstream>
#include <iostream>
using namespace std;

cat::cat(char* filename){
	file.open(filename, fstream::in);
}
void cat::printFile(){
	if(file.good()){
		char c;
	  	while (file.get(c))// loop getting single characters
	    	cout << c;
	}else{
		cout << "Error al abrir archivo, archivo no encontrado" << endl;
	}
}