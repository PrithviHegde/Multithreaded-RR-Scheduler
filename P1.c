#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>

#define ll long long

int NUM_THREADS=10;

int a,b,c;

typedef struct 
{
    int cur, rows, cols, *shm, *check;
    char line[];
} file_arg;
        
void *func(void *args)
{
	file_arg* data = (file_arg*)args;
	char *ptr = data->line;
	
	int i= 0;
	while(*ptr)
	{
		if (*ptr>='0'&&*ptr<='9') 
		{
      ((ll(*)[data->cols])data->shm)[data->cur][i] = 0;
      while (*ptr>='0'&&*ptr<='9'){
       ((ll(*)[data->cols])data->shm)[data->cur][i] *= 10; 
       ((ll(*)[data->cols])data->shm)[data->cur][i] += *ptr++ - '0';
      }
      i++;
      if (i>=data->cols) break; 
  	}
  	else
  		ptr++;		
	}
	if(i==data->cols)
		(data->check)[data->cur]=1;
	return NULL;
}

int main(int argv, char *argc[])
{
	//File pointers
	FILE *fptr1 = fopen(argc[4], "r"); 
	FILE *fptr2 = fopen(argc[5], "r"); 

	// ftok to generate unique key
	key_t f1 = ftok(".",65);
	key_t f2 = ftok(".",64);
	key_t f3 = ftok(".",63);
	key_t f4 = ftok(".",62);
	
	//Creating input matrices
	a = atoi(argc[1]);
	b = atoi(argc[2]);
	c = atoi(argc[3]);
	//if(argv==9)
	//NUM_THREADS = atoi(argc[6]);
	
	// shmget returns an identifier in shmid
	int shmid1 = shmget(f1, a*b*sizeof(ll), 0666|IPC_CREAT);
	int shmid2 = shmget(f2, b*c*sizeof(ll), 0666|IPC_CREAT);
	int shmid3 = shmget(f3, a*sizeof(int), 0666|IPC_CREAT);
	int shmid4 = shmget(f4, c*sizeof(int), 0666|IPC_CREAT);

	// shmat to attach to shared memory
	ll (*in1)[b] = (void*) shmat(shmid1,(void*)0,0);
	ll (*in2)[b] = (void*) shmat(shmid2,(void*)0,0);
	int *check1 = (void*) shmat(shmid3,(void*)0,0);
	int *check2 = (void*) shmat(shmid4,(void*)0,0);
	
	memset(check1, 0, sizeof(check1));
	memset(check2, 0, sizeof(check2));
	
	ll num;
	for(int i=0;i<b;i++)
	{
		fscanf(fptr1, "%lld", &num);
		in1[0][i] = num;
		
	}
	check1[0]=1;
	
	pthread_t id[NUM_THREADS];
	int cur1=0, cur2=1;
	
	while(cur1<c)
	{
		int i;
		for(i=0; cur1<c && i<NUM_THREADS ; i++,cur1++)
		{
			char *line;
			size_t size=0;
			file_arg *inp;

			if(getline(&line, &size, fptr2))
			{
				inp = malloc(sizeof(file_arg)+strlen(line)+1);		
				strcpy(inp->line, line); 
			}
			else
				inp = malloc(sizeof(file_arg));
			
			inp->cur=cur1; inp->rows=c; inp->cols=b; inp->shm=(int*)in2; inp->check=check2;
			
			if(pthread_create(&id[i], NULL, func, (void *)inp)!=0)
			{
				printf("Thread Creation Failed!");
			}
		}

		for(int j=0; j<i; j++)
		{
			pthread_join(id[j], NULL);
		}
	}
	
	while(cur2<a+1)
	{
		int i;
		for(i=0; cur2<a+1 && i<NUM_THREADS ; i++,cur2++)
		{
			char *line;
			size_t size;
			file_arg *inp;

			if(getline(&line, &size, fptr1))
			{
				inp = malloc(sizeof(file_arg)+strlen(line)+1);		
				strcpy(inp->line, line); 
			}
			else
				inp = malloc(sizeof(file_arg));
			
			inp->cur=cur2-1; inp->rows=a; inp->cols=b; inp->shm=(int*)in1; inp->check=check1;
			
			if(pthread_create(&id[i], NULL, func, (void *)inp)!=0)
			{
				printf("Thread Creation Failed!");
			}
		}

		for(int j=0; j<i; j++)
		{
			pthread_join(id[j], NULL);
		}
	}
	
	//detach from shared memory 
	shmdt(in1);
	shmdt(in2);
	shmdt(check1);
  shmdt(check2);
  
	return 0;
}
