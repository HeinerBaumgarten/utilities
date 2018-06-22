/*
 ============================================================================
 Name        : timestamp2.c
 Author      : Heiner Baumgarten
 Version     : 1.0
 Copyright   : You can do what you want
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define TS_SIZE 28

void printtimestamp(FILE *fp, struct timeval tp,struct tm *t,char timestamp[TS_SIZE]);
void settimezone();

int main(void) {

	struct timeval tp;
	struct tm *t ;
	char timestamp[] = "YYYY-MM-DD-Thh:mm:ss.mssUTC";
	FILE *filepointer;

	filepointer = fopen("./test.txt","w+");
	gettimeofday(&tp, NULL);
	t = localtime(&tp.tv_sec);
	settimezone();
	for(int i= 0; i < 1000; i++){
		printtimestamp(filepointer,tp,t, timestamp);
	}
	fclose(filepointer);

	return EXIT_SUCCESS;
}

void printtimestamp(FILE *fp,struct timeval tp, struct tm *t,char timestamp[TS_SIZE]){

	gettimeofday(&tp, NULL);
	t = localtime(&tp.tv_sec);
	strftime(timestamp,TS_SIZE,"%Y-%m-%dT%H:%M:%S",t);
	fprintf(fp,"%s.%03d%s\t",timestamp,(int)(tp.tv_usec/1000),t->tm_zone);
}

void settimezone(){
	setenv("TZ","UTC0",1);
	tzset();
}
