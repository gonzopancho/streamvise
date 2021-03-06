#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


/* Build with 
gcc -D NOLIB cit-helper.c  -o testcit-helper && ./testcit-helper
to test things with main function, 

or with
gcc -c cit-helper.c && gcc -o libcit-helper.so cit-helper.o --share 
to use it as a library..
*/


/* PART1 : Generic functions
*/

int myprint(int toprint){
	//printf("here: %i", toprint);
	//system("echo ok > /tmp/myprint");
	return 2*toprint;
}


int runsystem(char* cmd){
	printf("*****   Running system command: %s\n",cmd);
	system(cmd);
//TODO: replace system with exec, start new process etc..
	return 0;
}



char *readsetting(char *filename, char *setting, char *delimiter){

	printf("Looking for string %s in file %s\n", setting, filename);
	FILE *file;
	FILE *filetmp;
        size_t len = 0;
	char *line = NULL;
	char *ret = "Setting not found";

	if (filetmp = fopen("/tmp/filetmp","w"))
	if (file = fopen(filename,"r") )// Open the file
	{
	        while ((getline(&line, &len, file)) != -1) 
     		{
// maybe tmp should be defined as char *tmp[1] ???
			char tmp[1];				
			sprintf(tmp, "%.*s\n", 1, &line[0]);

			//Now do 2 checks:
			//1. First character of line is not "#", so this is not a comment line
			//2. Try to find the string we're looking for. If not null, we found it successfully.
			if (strstr(tmp, "#") == NULL && strstr(line, setting) != NULL){ 
				fprintf(filetmp, "%s", line);
				strtok(line, delimiter);
				ret = strdup(strtok(NULL , delimiter));

			}else{ //This is not the line we were looking for. Rewrite it just to move the fprintf pointer.
				fprintf(filetmp, "%s", line);
			}			
      		}
		fclose(file);
		fclose(filetmp);
		move_file("/tmp/filetmp", filename);
		return ret;
	}
	else{
		printf("Error opening file %s!\n", filename);
		return "fail";
	}


}



int writesetting(char *filename, char *setting, char *towrite, char *delimiter){

//     	char line[128]; //Will hold one line of the conf file
//	char filename[] = "/tmp/dhcpconf"; //Which file to open

	printf("Will write string %s in file %s, as setting %s\n", towrite, filename,setting);
	FILE *file;
	FILE *filetmp;
        size_t len = 0;
	char *line = NULL;
	int found = 0; // NOT found by default

	if (filetmp = fopen("/tmp/filetmp","w"))
	if (file = fopen(filename,"r") )// Open the file
	{
	        while ((getline(&line, &len, file)) != -1) 
     		{
// maybe tmp should be defined as char *tmp[1] ???
			char tmp[1];				
			sprintf(tmp, "%.*s\n", 1, &line[0]);

			//Now do 2 checks:
			//1. First character of line is not "#", so this is not a comment line
			//2. Try to find the string we're looking for. If not null, we found it!
			if (strstr(tmp, "#") == NULL && strstr(line, setting) != NULL){ 
				//Is this about a dns setting?
//				if (strstr(setting, "option  dns") != NULL) {
//				  printf("This is a dns request.");
					
//				  strtok(line, delimiter); //Skip the line's first word
//				  strtok(NULL, delimiter); //Skip the line's second word
				  //If the third word is the same as the one we would write...
//				  if (strstr(towrite, strdup(strtok(NULL , delimiter))) != NULL) {
//				    fprintf(filetmp, "%s", line); //Just rewrite the line
//				  }else{//Append the line to the file
//				    fprintf(filetmp, "%s", line); //First rewrite the line
				    //Then append the new line:
//				    fprintf(filetmp,"%s%s%s\n", setting, delimiter, towrite);
//				  }
//				}else{
					fprintf(filetmp,"%s%s%s\n", setting, delimiter, towrite);
//					found = 1;
//				}
			}else{ //This is not the line we were looking for. Rewrite it just to move the fprintf pointer.
				fprintf(filetmp, "%s", line);
			}			
      		}
//		if (found == 0) //If setting has not been found in file, write it
//			fprintf(filetmp,"%s%s%s\n", setting, delimiter, towrite);
		fclose(file);
		fclose(filetmp);
		move_file("/tmp/filetmp", filename);
		return 1;
	}
	else{
		printf("Error opening file %s!\n", filename);
		return -1;
	}


}

move_file(char *src, char *dst){
	FILE *f1,*f2;
	char ch;
//	printf("moving from %s to %s\n",src, dst);

	f1=fopen(src,"r");
	if(f1==NULL)
		printf("Can't open the file");
	else {
		f2=fopen(dst,"w");
		while((ch=getc(f1))!=EOF)
			putc(ch,f2);
//		printf("One File moved\n");
		fclose(f2);
		remove(src);
	}
	fclose(f1);
	return 0;
}


/* PART2 : File-specific wrapper functions
*/

int writeudhcpd(char *setting, char *towrite){
	char file[]="/tmp/udhcpd.conf";
	return writesetting(file, setting, towrite, "	");
}

char *readudhcpd(char *tofind){
	char file[]="/tmp/udhcpd.conf";
	return readsetting(file, tofind, "	");
}


/* PART3 : Test functions
*/

#ifdef NOLIB //If running in debug mode, main function will be compiled.

int main(){ //Used only for debugging purposes.
//	printf("Main() running!\n");
//	printf("Reading settings: %s\n", readsetting("/tmp/udhcpd.conf", "field3"));


	writeudhcpd("option  dns", "5");
//	printf("FOUND: %s\n", readudhcpd("start"));



	return 0;
}
#endif
