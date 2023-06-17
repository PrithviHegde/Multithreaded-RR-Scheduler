#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int timeQuantum = 2000;
int main(int argc, char *argv[]){
	FILE *fptr1 = fopen("turnAround.csv", "a+");
	FILE *fptr2 = fopen("waiting.csv", "a+");
	FILE *fptr3 = fopen("switchOverhead.csv", "a+");   
	struct timespec st1={0,0}, st2={0,0}, tp1={0,0}, tp2={0,0},  s1={0,0}, s2={0,0}, quant1={0,0}, quant2={0,0};   
	pid_t pid, pid1;
	char *argv3[] = {argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]};
	signal(SIGCHLD,SIG_IGN);
	
	double switchTime = 0, turnAroundP1=0, turnAroundP2=0, burstp1=0, burstp2=0;
	
	pid = fork();
	if(pid<0) perror("fork");
	else if(pid>0)
	{
	  pid1=fork();
	  if(pid1<0) perror("fork1");
	  else if(pid1==0){
	  	kill(getpid(),SIGSTOP);
      execl("a", "./a", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
  	}
		else
		{
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &s1);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &s2);
		  while((kill(pid1,SIGSTOP)!=-1) && (kill(pid,SIGSTOP)!=-1))
		  {
		  	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &st1);
		    kill(pid, SIGSTOP);
		    kill(pid1,SIGCONT);
		    
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &st2);
		    switchTime += ((double)st2.tv_sec*1.0e9 + st2.tv_nsec) - ((double)st1.tv_sec*1.0e9 + st1.tv_nsec);
		    
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &quant1);
		    usleep(timeQuantum);
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &quant2);
		    burstp1 += ((double)quant2.tv_sec*1.0e9 + quant2.tv_nsec) - ((double)quant1.tv_sec*1.0e9 + quant1.tv_nsec);
		    
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &st1);
		    kill(pid1, SIGSTOP);
		    kill(pid,SIGCONT);
		    
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &st2);
		    switchTime += ((double)st2.tv_sec*1.0e9 + st2.tv_nsec) - ((double)st1.tv_sec*1.0e9 + st1.tv_nsec);
		    
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &quant1);
		    usleep(timeQuantum);
		    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &quant2);
		    burstp2 += ((double)quant2.tv_sec*1.0e9 + quant2.tv_nsec) - ((double)quant1.tv_sec*1.0e9 + quant1.tv_nsec);
		  }
		  
		  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp1);
		  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &quant1);
		  if(kill(pid,SIGCONT)!=-1){
		    kill(pid,SIGCONT);
		  }
		  while(kill(pid,SIGCONT)!=-1);
		  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp2);
		  turnAroundP1 = ((double)tp1.tv_sec*1.0e9 + tp1.tv_nsec) - ((double)s1.tv_sec*1.0e9 + s1.tv_nsec);
		  turnAroundP2 = ((double)tp2.tv_sec*1.0e9 + tp2.tv_nsec) - ((double)s2.tv_sec*1.0e9 + s2.tv_nsec);
	  }
	}
	else{
			kill(getpid(),SIGSTOP);
		  execl("b", "./b", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], NULL);
	}
	wait(NULL);
	wait(NULL);
	burstp2 += ((double)tp2.tv_sec*1.0e9 + tp2.tv_nsec) - ((double)quant1.tv_sec*1.0e9 + quant1.tv_nsec);
	int workload=atoi(argv[1])*atoi(argv[2])*atoi(argv[3]);
	
	fprintf(fptr1, "%d,%lf,%lf\n", workload, turnAroundP1, turnAroundP2);
	fprintf(fptr2, "%d,%lf,%lf\n", workload, turnAroundP1-burstp1, turnAroundP2-burstp2);
	fprintf(fptr3, "%d,%lf\n", workload,switchTime);
	return 0;	
}

