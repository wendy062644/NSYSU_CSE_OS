#include <unistd.h>
#include <stdlib.h> /* for atoi which converts to integer*/
#include <lib.h>
#include <stdio.h>
#include <time.h>
int main(int argc, char **argv){
	int counter = 0;
	int counter2 = 0;
	FILE *log;
	log = fopen("DynamicLog.txt","a");
	pid_t pid = getpid();
	time_t current_time = time(NULL);
	fprintf(log,"CPU BOUND with initially 5 tickets pid %d started at %s\n",pid,ctime(&current_time));
	while(counter2<8){

	counter =0;

	while(counter <1111111111){
	int x =10;
	int y =20;
	x = y*x;
	counter++;
	}
	counter2++;}
	current_time = time(NULL);
	printf("CPU BOUND with initial 5 tickets: pid %d has finished... \n",pid);
	fprintf(log,"CPU BOUND with initial 5 tickets:%d has finished at %s\n",pid,ctime(&current_time));
}
