#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include<omp.h>
#include<malloc.h>
#include "md5sum.h"
#include<conio.h>
typedef struct file_properties{
	char name[10000];
	char *hash;
} FS;
FS nul = {"",""};
//Global Variables Declaration
FS files[1000];
int total_files = 0;



void get_directory_files(char directory[]){
	DIR *d; 
	FS *tempfiles;
	int k = 0, total = 0;
	struct dirent *dir;
	d = opendir(directory);
	if(d){
		while((dir = readdir(d))!= NULL){
			if(dir->d_name[0] == '.' || (dir->d_name[0] == '.') && dir->d_name[1] == '.' ) continue;
			total++;
			strncpy(files[k].name,dir->d_name,1000);
			k++;
		}
		closedir(d);
	}
	printf("Total Files : %d\n",total);
	total_files = total;
	//return files;	
}


int main(){
	get_directory_files(".");
	int n = total_files;
	int i =0,j;
	#pragma omp parallel for 
	for(i=0;i<n;i++){
		FILE *fp = fopen(files[i].name,"r");
		files[i].hash = (char *) malloc (32*sizeof(char *));
		sum(fp,files[i].name,files[i].hash);
		fclose(fp);
	}
	#pragma omp parallel for collapse(2)
	for(i=0;i<n;i++){
		for(j=0;j<n-1;j++){          
		if(strcmp(files[i].hash,"1B2M2Y8AsgTpgAmY7PhCfg") == 0) continue; //if directory
		if(strcmp(files[j].hash,"1B2M2Y8AsgTpgAmY7PhCfg") == 0) continue;
			if( i!=j && strcmp(files[i].hash,files[j].hash) == 0 ){
				printf("\nDuplicate File at %s",files[j].name);
				getch();
				remove(files[j].name);
				files[j] = nul;
			}
		}
	}
	getch();
	return 1;
}


