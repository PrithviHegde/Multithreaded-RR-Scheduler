#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#define ll long long

int NUM_THREADS=10;

int a, b, c, *check1, *check2;

typedef struct 
{
  int tid, *shm1, *shm2, *out;
} file_arg;
    
void *func(void *args)
{
	file_arg* data = (file_arg*)args;
	
	int tid=data->tid;
	ll (*in1)[b] = (ll(*)[b])data->shm1, (*in2)[b] = (ll(*)[b])data->shm2;
	ll (*out)[c] = (ll(*)[c])data->out;
	
	for(int i=0; i<a; i++)
	{
		for(int k=0; k<c; k++)
		{
			if((i*c+k)%NUM_THREADS == tid)
				while(1)
				{
					if(check1[i]==1 && check2[k]==1)
					{
						for(int j=0; j<b; j++)
						{
							out[i][k]+=in1[i][j]*in2[k][j];
						}
						break;
					}
				}
		}
	}
}
  
int main(int argv, char *argc[])
{
  time_t t,t1;
  
  //File pointers
	FILE *fptr1 = fopen(argc[4], "r"); 
	FILE *fptr2 = fopen(argc[5], "r"); 
	FILE *fptr3 = fopen(argc[6], "w");

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
	//NUM_THREADS = atoi(argc[7]);
	
  // shmget returns an identifier in shmid
	int shmid1 = shmget(f1, a*b*sizeof(ll), 0666|IPC_CREAT);
	int shmid2 = shmget(f2, b*c*sizeof(ll), 0666|IPC_CREAT);
	int shmid3 = shmget(f3, a*sizeof(int), 0666|IPC_CREAT);
	int shmid4 = shmget(f4, c*sizeof(int), 0666|IPC_CREAT);
	
  // // shmat to attach to shared memory
  int* in1 = (void*) shmat(shmid1,(void*)0,0);
	int* in2 = (void*) shmat(shmid2,(void*)0,0);
	check1 = (void*) shmat(shmid3,(void*)0,0);
	check2 = (void*) shmat(shmid4,(void*)0,0);

	//Multiplying the Matrices
	ll out[a][c];
	
	for(int i=0; i<a; i++)
		for(int j=0; j<c; j++)
			out[i][j]=0;
	
	pthread_t id[NUM_THREADS];
	
	t=time(NULL);
	for(int i=0; i<NUM_THREADS; i++)
	{
		file_arg *inp = malloc(sizeof(file_arg));
		inp->tid=i;inp->shm1=in1;inp->shm2=in2;inp->out=(int*)out;
		
		pthread_create(&id[i], NULL, func, (void *)inp);
	}
	
	for(int i=0; i<NUM_THREADS; i++)
		pthread_join(id[i], NULL);
		
	t1=time(NULL);
	
	//printf("%lf\n", difftime(t1, t));	
	
	for(int i=0; i<a; i++)
	{
		for(int j=0; j<c;j++)
		{
			fprintf(fptr3, "%lld ", out[i][j]);
		}
		fprintf(fptr3, "\n");
	}
  //detach from shared memory 
  shmdt(in1);
  shmdt(in2);
  shmdt(check1);
  shmdt(check2);

  // destroy the shared memory
  shmctl(shmid1,IPC_RMID,NULL);
  shmctl(shmid2,IPC_RMID,NULL);
  shmctl(shmid3,IPC_RMID,NULL);
  shmctl(shmid4,IPC_RMID,NULL);

  
  return 0;
}
