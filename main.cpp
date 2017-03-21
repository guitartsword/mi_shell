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
#include <fcntl.h>
#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

//INLCUDING COMMANDS
//#include "commands/cat.h"

using namespace std;

enum TipoComando{PIPE, REDIRECT, NEITHER};

int executeCommand(vector<string>&);
vector<string> getTokens(string, char);
char** vectorToCharPP (vector<string> tokenized);
TipoComando identificarComando(int, char**, char**, char**);
void Pipe(string, char**, char**, char**);
void Redirect(string, char**, char**, char**);
void Neither(string, int, char**, char**, char**);

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
		if(tokens[0] == "cd"){
			if(tokens.size()== 2){
				chdir(tokens[1].c_str());
			}
			return 0;
		}else{

			char *comando1[50], *comando2[50];

			TipoComando tipo_comando;
			char** argvData = vectorToCharPP (tokens);
			int argcData = tokens.size();
					
			//string commandPath = PATH + argvData[0];

			tipo_comando = identificarComando(argcData, argvData, comando1, comando2);

			if(tipo_comando == PIPE){
				cout << "Entra a Pipe" << endl;

				Pipe(PATH, argvData, comando1, comando2);

			}else if(tipo_comando == REDIRECT){
				cout << "Entra a REDIRECT" << endl;

				Redirect(PATH, argvData, comando1, comando2);

			}else{
				cout << "entra a NEITHER" << endl;

				Neither(PATH, argcData, argvData, comando1, comando2);
			}
		}
		/*int ppid, chpid;
		
		if(tokens[0] == "cd"){
			if(tokens.size()== 2){
				chdir(tokens[1].c_str());
			}
			return 0;

		}else{
			if((chpid=fork()) == 0){
				char** argvData = vectorToCharPP (tokens);
				int argcData = tokens.size();
				
				string commandPath = PATH + argvData[0];

				childErr = execvp(commandPath.c_str(), argvData);
				cout << "Comando " << argvData[0] <<": Fallido" << endl;

				return childErr;
			}else{
				wait(NULL);
				return 0;
			}
			return -1;
		}*/

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

TipoComando identificarComando(int argcData, char** argvData, char** comando1, char** comando2){
	cout << "Entra a identificar" << endl;

	TipoComando tipo = NEITHER;
	int temp = -1;

	for(int i = 0; i < argcData; i++){
		if(strcmp(argvData[i], "|") == 0){
			tipo = PIPE;
			temp = i;

		}else if (strcmp(argvData[i], ">>") == 0){
			tipo = REDIRECT;
			temp = i;
		}
	}

	if(tipo != NEITHER){
		for(int i = 0; i < temp; i++){
			comando1[i] = argvData[i];
		}

		int count = 0;
		for(int i=temp+1; i<argcData; i++){
			comando2[count] = argvData[i];
			count++;
		}

		comando1[temp] = NULL;
		comando2[count] = NULL;
	}

	return tipo;
}

void Pipe(string PATH, char** argvData, char** comando1, char** comando2){

	int a[2];
	pid_t pid;

	pipe(a);

	if(fork() == 0){//PROCESO PADRE 
		string commandPath = PATH + comando2[0];

		dup2(a[0], 0);
		close(a[1]);

		execvp(commandPath.c_str(), argvData);//No estoy segura de ese argvData en el execvp
		perror("Error en el execvp");
	}else if((pid == fork()) == 0){//PROCESO HIJO
		string commandPath = PATH + comando1[0];

		dup2(a[1], 1);
		close(a[0]);

		execvp(commandPath.c_str(), argvData);
		perror("Error en el execvp");
	} else {
		waitpid(pid, NULL, 0);
	}

}

void Redirect(string PATH, char** argvData, char** comando1, char** comando2){
	int contador;
	int file;
	int a[2];
	char caracter;
	pid_t pid;

	pipe(a);

	if(fork() == 0){
		string commandPath = PATH + comando2[0];

		file = open(commandPath.c_str(), O_RDWR | O_CREAT, 0666);

		if(file < 0){
			printf("Error: %s\n", strerror(errno));
		}

					//dup(a[0]);
		dup2(a[0], 0);
		close(a[1]);

		while((contador = read(0, &caracter, 1)) > 0){
			write(file, &caracter, 1);
		}

		execlp("echo", "echo", NULL);
	}else if((pid = fork()) == 0){
		string commandPath = PATH + comando1[0];

		dup2(a[1], 1);
		close(a[0]);

		execvp(commandPath.c_str(), comando1);//probar sin argvData
		perror("execvp error en el execvp");
	}else{
		waitpid(pid, NULL, 0);
		close(a[0]);
		close(a[1]);
	}
}

void Neither(string PATH, int argcData, char** argvData, char** comando1, char** comando2){
	const char *amperson;
	pid_t pid;
	amperson = "&";
	bool encontro = false;

	if(strcmp(argvData[argcData-1], amperson) == 0){
		encontro = true;
	} 

	pid = fork();

	if(pid < 0){
		perror("Hubo un error");
				
	}else if(pid == 0){
					
		if(encontro){
			argvData[argcData-1] = NULL;
			argcData--;
		}

		execvp(argvData[0], argvData);
		perror("Error en el execvp");
				
	}else if( !encontro){
		waitpid(pid, NULL, 0);
	}
}