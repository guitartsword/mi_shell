#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib> //C standard library <stdlib.h>, /* atoi */
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif


//INLCUDING COMMANDS
#include "cat.h"

using namespace std;

int executeCommand(vector<string>&);
vector<string> getTokens(string, char);

int main(int argc, char const *argv[]){
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
    if(tokens[0] == "help"){
        if(tokens.size() == 1){
            cout << "-----AYUDA-----\n"
            << "por ahora no es de mucha ayuda, pero\n" 
            << "si ocupan la lista de comandos disponible\n"
            << "utiliza 'help list' para mostrar la lista de comandos disponibles\n" << endl;
            return 0;
        }
        if(tokens[1] == "list"){
                cout
            << "cat [archivo]" << endl
            << "help (list)" << endl
            << "exit [numero]" << endl;    
        }else{
            cout << "No hay ayuda para comando listado" << endl;
        }
        return 0;
    }
    if(tokens[0] == "exit"){
        if(tokens.size() == 1){
            return 1;
        }
        return atoi(tokens[1].c_str());
    }
    if(tokens[0] == "cat"){
        if(tokens.size() == 1){
            cout << "Uso incorrecto de cat, porfavor especifique archivo\n"
            << "Ejemplo: cat archivo.txt, cat [direccion/del/archivo.cpp]";
            return 0;
        }
        int controlador = 1;
        //int i = 0;
        do{
	        //if(controlador == 0 || tokens[i] == ' '){
                	FILE *file;
                	char line[100];
                	file = fopen(tokens[controlador].c_str(),"r");
                	while(fscanf(file,"%[^\n]\n",line)!=EOF){
          	      		printf("%s\n", line);
                	}	
                	fclose(file);
                 	//arg = strtok(NULL, DELIMS);
               //}
          //     i++;
               controlador++;
        }while(controlador!=tokens.size());
	
        /*char * filename = new char [tokens[1].length()+1];
        strcpy(filename,tokens[1].c_str());
        cout << filename << endl;
        cat fileToPrint (filename);
        fileToPrint.printFile();
        delete[] filename;
        return 0;*/
    }
    else if (tokens[0] == "rm"){
	if(tokens.size() == 1){
		cout<<"Uso incorrecto de rm, muy pocos argumentos"<<endl;
		return 0;
	}
	else{
		if(tokens[1]=="-R"){
			if(tokens.size()==2){
			cout<<"uso incorrecto de rm -R, muy pocos argumentos"<<endl;
			}else{
				if (fork() == 0) {// proceso hijo
                        		execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/rm-R",tokens[2].c_str(),tokens[2].c_str(), (char *)0);
                		}
			}
		}else{
			if(fork()==0){
				execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/rm",tokens[1].c_str(),tokens[1].c_str(), (char *)0);
			}	
		}
	}
    }else if(tokens[0]=="ls"){
	if(tokens.size()==2){
		if(tokens[1]=="-m"){
			if(fork() == 0){
				execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/ls-m", "./ls-m", (char *)0);
			}
		
		}
		else if(tokens[1]=="-l"){
			if(fork() == 0){
				//execlp("./ls-l", "./ls-l", (char *)0);
				execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/ls-l", "./ls-l", (char *)0);
			}
		}
	}else{
		if(fork() == 0){
			execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/ls","./ls", (char *)0);
		}
	}
    }
    else if(tokens[0]=="cd"){
	if(tokens.size()== 1){
		cout<<"Uso incorrecto de cd, muy pocos argumentos"<<endl;
		return 0;
	}
	else{
		chdir(tokens[1].c_str());
	}
    }
    else if(tokens[0]=="rmdir"){
	if(tokens.size()==1){
		cout<<"Uso incorrecto de rmdir, muy pocos argumentos"<<endl;
		return 0;
	}else{
		if(fork() == 0){
			execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/rmdir","./rmdir",tokens[1].c_str(), (char *)0);
		}
	}
    }
    else if(tokens[0]=="ps"){
	if(tokens.size()==2){
		if(tokens[1]=="-e"){
			if(fork() == 0){
				execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/ps-e","./ps-e", (char *)0);
			}
		}
	}
    }else if(tokens[0]=="ln"){
	if(tokens.size()==1){
		cout<<"Error, hacen falta argumentos"<<endl;
		return 0;
	}else{
		if(tokens[1]=="-s"){
			if(fork() == 0){
				execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/ln","./ln",tokens[2].c_str(), tokens[3].c_str(), (char *)0);
			}
			
		}
	}
    }else if(tokens[0]=="uname"){
	if(tokens.size()==1){
		cout<<"Error, faltan argumentos"<<endl;
	}else{
		if(fork() == 0){
			execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/uname","./uname",tokens[1].c_str(), (char *)0);
		}
		
	}
    }else if(tokens[0]=="chmod"){
	if(tokens.size()==1){
		cout<<"Error, faltan argumentos"<<endl;
		return 0;
	}
	else{
                int controlador = 0;
                int i = 0;
                int mode;
                do{
                        //if(arg[i]==' '){
                                controlador++;
                                if(controlador==1){
                                        mode = strtol(tokens[1].c_str(), 0, 8);
                                }
                                if(controlador==2){
                                        if(chmod(tokens[2].c_str(), mode) != 0){
                                                perror("Command failed");
                                        }
                                }
                              //  arg = strtok(NULL, DELIMS);
                        //}
                        i++;
                }while(controlador!=tokens.size());
                if(controlador<2){
                        fprintf(stderr, "chmod missing argument .\n");
                }

	}
    }else if(tokens[0]=="mkdir"){
		if(tokens.size()==1){
			cout<<"Error, faltan argumentos"<<endl;
			return 0;
		}
		else{
			if(fork() == 0){
				execlp("/home/isaac/Desktop/Trabajos/Sistemas_Operativos_1/scripts/Isaias/mi_shell/mkdir","./mkdir",tokens[1].c_str(), (char *)0);
			}
			
		}
    }
    else
    	cout << tokens[0] << ": comando no encontrado" << endl;
    return 0;
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
