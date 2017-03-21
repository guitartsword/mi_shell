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

int check_if_number (char *str)
{
  int i;
  for (i=0; str[i] != '\0'; i++)
  {
    if (!isdigit (str[i]))
    {
      return 0;
    }
  }
  return 1;
}

#define MAX_BUF 1024
#define INT_SIZE_BUF 6
#define PID_LIST_BLOCK 32
#define UP_TIME_SIZE 10
const char *getUserName(int uid)
{
  struct passwd *pw = getpwuid(uid);
  if (pw)
  {
    return pw->pw_name;
  }

  return "";
}

void pidaux ()
{
  DIR *dirp;
  FILE *fp;
  struct dirent *entry;
  char path[MAX_BUF], read_buf[MAX_BUF],temp_buf[MAX_BUF];
  char uid_int_str[INT_SIZE_BUF]={0},*line;
  char uptime_str[UP_TIME_SIZE];
  char *user,*command;
  size_t len=0;
  dirp = opendir ("/proc/");
  if (dirp == NULL)
  {
         perror ("Fail");
         exit(0);
  } 
  strcpy(path,"/proc/");
  strcat(path,"uptime");
 
  fp=fopen(path,"r");
  if(fp!=NULL)
  {
	getline(&line,&len,fp);
	sscanf(line,"%s ",uptime_str);
  }
  long uptime=atof(uptime_str);
  long Hertz=sysconf(_SC_CLK_TCK); 
  strcpy(path,"/proc/");
  strcat(path,"meminfo");

  fp=fopen(path,"r");
  unsigned long long total_memory;
  if(fp!=NULL)
  {
	getline(&line,&len,fp);
	sscanf(line,"MemTotal:        %llu kB",&total_memory);
  }	

  while ((entry = readdir (dirp)) != NULL)
  {
    if (check_if_number (entry->d_name))
    {
	strcpy(path,"/proc/");
	strcat(path,entry->d_name);
	strcat(path,"/status");
	unsigned long long memory_rss;
	fp=fopen(path,"r");
	unsigned long long vmsize;

	if(fp!=NULL)
	{
		vmsize=0;
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		getline(&line,&len,fp);
		sscanf(line,"Uid:    %s ",uid_int_str);
		getline(&line,&len,fp);
                getline(&line,&len,fp);
                getline(&line,&len,fp);
		getline(&line,&len,fp);
                getline(&line,&len,fp);
		sscanf(line,"VmSize:    %llu kB",&vmsize);
                getline(&line,&len,fp);
                getline(&line,&len,fp);
                getline(&line,&len,fp);
		getline(&line,&len,fp);
		sscanf(line,"VmRSS:     %llu kB",&memory_rss);
		
	}
	else
	{
		fprintf(stdout,"FP is NULL\n");
	}
	float memory_usage=100*memory_rss/total_memory;
	strcpy(path,"/proc/");
	strcat(path,entry->d_name);
	strcat(path,"/stat");
	fp=fopen(path,"r");
	getline(&line,&len,fp);
	char comm[10],state;
	unsigned int flags;
	int pid,ppid,pgrp,session,tty_nr,tpgid;
	unsigned long minflt,cminflt,majflt,cmajflt,utime,stime;
	unsigned long long starttime;
	long cutime,cstime,priority,nice,num_threads,itreavalue;
	sscanf(line,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld  %ld %llu",&pid,comm,&state,&ppid,&pgrp,&session,&tty_nr,&tpgid,&flags,&minflt,&cminflt,&majflt,&cmajflt,&utime,&stime,&cutime,&cstime,&priority,&nice,&num_threads,&itreavalue,&starttime);
	unsigned long total_time=utime+stime;
	total_time=total_time+(unsigned long)cutime+(unsigned long)cstime;
	float seconds=uptime-(starttime/Hertz);
	float cpu_usage=100*((total_time/Hertz)/seconds);
	if(isnan(cpu_usage))
	{
		cpu_usage=0.0;
	}
	if(isnan(memory_usage))
	{
		memory_usage=0.0;
	}
	strcpy (path, "/proc/");
	strcat (path, entry->d_name);
	strcat (path, "/comm");
	 
      	fp = fopen (path, "r");
      	if (fp != NULL)
      	{
        	fscanf (fp, "%s", read_buf);
		fclose(fp);
      	}
      	//char *userName=getUserName(atoi(uid_int_str));
        char userName[1000];
	//userName=getUserName(atoi(uid_int_str));
	strcpy(userName, getUserName(atoi(uid_int_str)));
     	if(strlen(userName)<9)
      	{
		user=userName;	
      	}
      	else
      	{
		user=uid_int_str;
      	}
      	fprintf(stdout,"%s %s %0.1f %0.1f %llu %llu %c %s\n",user,entry->d_name,cpu_usage,memory_usage,vmsize,memory_rss,state,read_buf);
     	 
    }
  } 
  closedir (dirp);
}

int removedir(char path[500]){
        int counter =1;
        DIR *pdir = NULL;
        struct dirent *pent = NULL;
        struct stat eStat;
        pdir = opendir(path);
        char x[500];
        if ( pdir == NULL){
                return -1;
        }
        while ( (pent=readdir(pdir)) != NULL ){
                if((strcmp((pent->d_name),".")==0)||(strcmp((pent->d_name),"..")==0)){
                        continue;
                }
                else{
                        strcpy(x,path);
                        path=strcat(path,"/");
                        path=strcat(path,pent->d_name);

                        if(stat(path, &eStat)){
                                printf("ERROR: %s... Meaning it can be a file(Most certainly)\n", strerror(errno));
                        }
                        else{
                                if(S_ISDIR(eStat.st_mode)){
                                        removedir(path);
                                        strcpy(path,x);
                                }
                                else{
                                        unlink(path);
                                        strcpy(path,x);
                                }
                        }
                }
        }
        if (!rmdir(path)) return -1; // delete the directory */
}

char pathname[MAXPATHLEN];

void die(char *msg){
  perror(msg);
  exit(0);
}

static int
one (const struct dirent *unused){
  return 1;
}



int main(int argc, char *argv[]){
	if(argc == 2){
		remove(argv[1]);
	}else{
		if(strcmp(argv[1], "-r")==0||strcmp(argv[1],"-R")==0){
			char* directory = argv[2];//strtok(0, DELIMS);
        		if(!directory) fprintf(stderr, "rm -R missing argument. \n");
        		else{
                		int ret;
                       		char *path_dir=malloc(500);
                        	//char *path_dir=directory;
                        	//char path[500];
                        	//printf("enter Directory name\n");
                        	//scanf("%s",path);
                        	strcpy(path_dir,directory);
                        	ret=removedir(path_dir);
                        	//printf("\n %d", ret);
                	}

		}
	}
}
