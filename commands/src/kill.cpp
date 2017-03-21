#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]){
	if(argc > 0){
		if(argc == 3){
			pid_t pid;
			sscanf(argv[2],"%d",&pid);
			bool equalString = !(strcmp(argv[1],"-9"));
			
			if(equalString){
				kill(pid, 9);
			}else{
				printf("%s\n", "Porfavor utilizar -9 como opcion");
			}

		}else{
			perror("Porfavor utilizar -9 como opcion");
			return -1;
		}
	}

	return 0;
}