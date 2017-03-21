#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/utsname.h>

int main(int argc, char *argv[]) {

   struct utsname buffer;

   errno = 0;
   if (uname(&buffer) != 0) {
      perror("uname");
      exit(EXIT_FAILURE);
   }
   if(strcmp(argv[1],"-s")==0)
   	printf("%s\n", buffer.sysname);
   else if(strcmp(argv[1],"-n")==0)
   	printf("%s\n", buffer.nodename);
   else if(strcmp(argv[1],"-r")==0)
   	printf("%s\n", buffer.release);
   else if(strcmp(argv[1],"-v")==0)
   	printf("%s\n", buffer.version);
   else if(strcmp(argv[1],"-m")==0)
   	printf("%s\n", buffer.machine);
  // printf("kernel      = %s\n", buffer.kernelname);
   else if(strcmp(argv[1],"-a")==0){
	printf("%s ", buffer.sysname);
        printf("%s ", buffer.nodename);
        printf("%s ", buffer.release);
  	printf("%s ", buffer.version);
	printf("%s ", buffer.machine);
        printf("\n");

   }else{
   	printf("%s\n", buffer.sysname);
   }
   #ifdef _GNU_SOURCE
      printf("domain name = %s\n", buffer.domainname);
   #endif

   return EXIT_SUCCESS;
}
