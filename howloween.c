///////////////////////////////////////////////////////////////////////
//
//	HOWLOWEEN.C									Written by: Adam Ickes
//
//	This program is intended for use only on a Raspberry Pi with the
//	mpg123 program installed. 
//
//	Howloween.c was designed to continuously play random halloween
//	oriented sound effects at random intervals.
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>

//Maximum wait (in seconds) between sounds
#define RAND_MIN_WAIT 5
#define RAND_MAX_WAIT 12
//Number of SFX files in the directory
#define NUMBER_OF_FILES 104

int gen_rand(int max);

void playRandomSound()
{
	char *file;
	char *fileName;
	char *fileLocation = "/home/pi/halloween/wav/";
	
	pid_t x; 
	int fileNumber = gen_rand((int)NUMBER_OF_FILES);
	int i = 0;
	DIR *pdir;
	struct dirent *pent;

	pdir=opendir("/home/pi/halloween/wav"); //"." refers to the current dir
	if (!pdir){
		printf ("opendir() failure; terminating");
		exit(1);
	}
	errno=0;
	while ((pent=readdir(pdir))){
		//printf("%s", pent->d_name);
		if (i==fileNumber) {
			fileName = pent->d_name;
		}
		i = i + 1;
	}
	if (errno){
		printf ("readdir() failure; terminating");
		exit(1);
	}
	closedir(pdir);
	
	//Combine fileLocation and fileName
	file = (char *)malloc(strlen(fileLocation) + strlen(fileName) +1);
	strcpy(file,fileLocation);
	strcat(file,fileName);
	//Play the sound
	x = fork();
	execlp("aplay","aplay",file);
}

int gen_rand(int max)
{
	int n;
	n=rand()%max;
	return(n);
}

int main()
{
    //Process events forever
	int waitTime;
    while (1) {
		//Delay before nextsound is played
		waitTime = gen_rand((int)RAND_MAX_WAIT - (int)RAND_MIN_WAIT);
		sleep((int)RAND_MIN_WAIT + waitTime);
		playRandomSound();
    }
	
	return 0;
}