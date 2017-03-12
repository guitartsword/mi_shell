#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h> //C standard library <stdlib.h>, /* atoi */

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
        char * filename = new char [tokens[1].length()+1];
        strcpy(filename,tokens[1].c_str());
        cout << filename << endl;
        cat fileToPrint (filename);
        fileToPrint.printFile();
        delete[] filename;
        return 0;
    }
    if(tokens[0] == "ln"){
       if(tokens.size() == 1){
            cout << "Falta comandos para el uso de ln, ln [opciones]\n"
                 << "opciones: -s"<<endl;
       }else{
         if(tokens[1] == "-s"){
            if(tokens.size() < 4){
                cout << "Falta el nombre del archivo existente o el nuevo archivo\n"
                     << "ln -s archivo.txt archivo2.txt"<<endl;
            }else{
                char * archivo = new char [tokens[2].length()+1];
                strcpy(archivo,tokens[2].c_str());

                char * linkarchivo = new char [tokens[3].length()+1];
                strcpy(linkarchivo,tokens[3].c_str());

                if(link(archivo, linkarchivo) < 0){
                    cout << "Error al linkear el archivo" << endl;
                }
            }
         }
      }
       return 0;
    }
    /*if(tokens[0] == "uname"){
        if(tokens.size() == 1){
            cout << "uname opcion \n"
                 << "opcion: -a -r -m -s" << endl;
        }else{
            struct utsname buffer;
            //uname unm;
            if(uname(&buffer) != 0){
                cout << "Eror en el uname" << endl;
            }else{
                if(tokens[1] == "-a"){
                    printf("%s ", buffer.sysname);  
                    printf("%s ", buffer.nodename); 
                    printf("%s ", buffer.release);  
                    printf("%s ", buffer.version);  
                    printf("%s ", buffer.machine);
                    printf("\n");
                }
                if(tokens[1] == "-r"){
                    printf("%s \n", buffer.release);
                }
                if(tokens[1] == "-m"){
                    printf("%s \n", buffer.machine);
                }
                if(tokens[1] == "-s"){
                    printf("%s \n", buffer.sysname);
                }
            }
        }
        return 0;     
    }*/
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
