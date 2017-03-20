#include <fstream>
#include <stdio.h>

using std::fstream;

int main(int argc, char const *argv[])
{
	if(argc > 1){
		for(int i = 1; i < argc; i++){
			fstream file(argv[i], fstream::in);
			if(file.good()){
				char c;
				while (file.get(c))// loop getting single characters
					printf("%c",c);
			}else{
				printf("Error al abrir archivo, archivo no encontrado\n");
			}
		}
	}
	
	return 0;
}