#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main(int argc, char *argv[]){
	mkdir(argv[1],0700);
}
