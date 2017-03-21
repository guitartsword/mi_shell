#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib> //C standard library <stdlib.h>, /* atoi */
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h> //C standard library <stdlib.h>, /* atoi */
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

//INLCUDING COMMANDS
//#include "commands/cat.h"

using namespace std;

int executeCommand(vector<string>&);
vector<string> getTokens(string, char);
char** vectorToCharPP (vector<string> tokenized);
string PATH, CURR_DIR;

int main(int argc, char const *argv[]){
	CURR_DIR = getenv("PWD");
	PATH = CURR_DIR+"/commands/bin/";
	int status = 0;
	while(!status) {
		cout << "mi_sh> ";
		string line;
		getline(cin,line);
		vector<string> tokens = getTokens(line, ' ');
		status = executeCommand(tokens);
	};
	if(status != 1){
		cerr << "ERROR " << status << ": al ejecutar el comando" << endl;
	}
	return 0;
}

/*
ejecuta los comandos conocidos por el programa
retorna 0 cuando todo salio bien y continuar con el programa
retorna 1 para salir del programa sin errores
retorna cualquier otro valor con error
*/
int executeCommand(vector<string>& tokens){
	if(tokens.size() == 0){
		cerr << "No hay commando a ejecutar" << endl;
		return 0;
	}
	int childErr;
	if(tokens.size() > 0){
			int ppid, chpid;
			string commandPath = PATH + tokens[0];
			char** argvData = vectorToCharPP (tokens);
		if(tokens[0] == "cd"){
			if(tokens.size()== 2){
				chdir(tokens[1].c_str());
			}
			return 0;

		}else{
			if((chpid=fork()) == 0){
				childErr = execvp(commandPath.c_str(), argvData);
				cout << "Comando " << argvData[0] <<": Fallido" << endl;

				return childErr;
			}else{
				wait(NULL);
				return 0;
			}
			return -1;
		}
	}

}

//retorna los tokens de la terminal
vector<string> getTokens(string toTokenize, char delimiter){
	vector<string> v;
	string::iterator stringIT = toTokenize.begin();
	for(string::iterator i = toTokenize.begin(); i != toTokenize.end(); i++){
		if(*i == delimiter){
			v.push_back(string(stringIT,i));
			stringIT=i+1;
		}
	}
	v.push_back(string(stringIT,toTokenize.end()));
	return v;
}
/*
retorna un char** al enviarle un vector<string>
vectorToCharPP = de vector a char puntero puntero 
*/
char** vectorToCharPP (vector<string> tokenized){
	char** tokens;
	int tokenAmount = tokenized.size();
	tokens = new char*[tokenAmount+1];
	for(int i = 0; i < tokenAmount; i++){
		tokens[i] = new char[tokenized[i].length()];
		strcpy(tokens[i],tokenized[i].c_str());
	}
	tokens[tokenAmount] = NULL;
	return tokens;
}
