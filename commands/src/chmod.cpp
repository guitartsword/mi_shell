#include <sys/stat.h>
#include <stdio.h>

// chmod -777 /arhchivo/directorio
int main(int argc, char const *argv[]){
	if(argc > 2){
		mode_t mode;
		sscanf(argv[1],"%d",&mode);
		return chmod(argv[2],mode);
	}
	return 0;
}