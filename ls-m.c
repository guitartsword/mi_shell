#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <malloc.h>
#include <sys/procfs.h>
#include <ctype.h>
#include <libgen.h>
#include <pwd.h>
#include <sys/utsname.h>
#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main(int argc, char *argv[]){
	char* arg = argv[1]; //strtok(0, DELIMS);
        char *curr_dir = NULL;
        DIR *dp = NULL;
        struct dirent *dptr = NULL;
        unsigned int count = 0;
        curr_dir = getenv("PWD");
        if(NULL == curr_dir){
                printf("\n ERROR : Could not get the working directory\n");
                return 0;
        }

        dp = opendir((const char*)curr_dir);
        if(NULL == dp){
                printf("\n ERROR : Could not open the working directory\n");
                return 0;
        }

        printf("\n");
        for(count = 0; NULL != (dptr = readdir(dp)); count++){
                printf("%s ,",dptr->d_name);
        }
        //printf("\n %u ", count);
        printf("\n");

	
}
