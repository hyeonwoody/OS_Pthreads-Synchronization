#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "SSU_Sem.h"

#define NUM_THREADS 3
#define NUM_ITER 10

SSU_Sem mySem[NUM_THREADS];


void *justprint(void *data)
{
  int thread_id = *((int *)data);
  for(int i=0; i < NUM_ITER; i++)
    {
      if (i==0){
        if (thread_id==0){}
        else
        SSU_Sem_down(&mySem[thread_id]);
      }
      //if (DEBUG1) {printf("i am  thread %d \n", thread_id); puts("access");}

      //if (DEBUG2) printf("Round %d ", i);
      printf("This is thread %d\n", thread_id);
      SSU_Sem_up(&mySem[thread_id+1==NUM_THREADS ? 0 : thread_id+1]);
      if (i==NUM_ITER-1) break;
      SSU_Sem_down(&mySem[thread_id]);
    }
  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];

  
  for (int i=0; i< NUM_THREADS; i++){
    
    SSU_Sem_init(&mySem[i], 0);
  }

  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      
      pthread_join(mythreads[i], NULL);
SSU_Sem_destroy(&mySem[i]);
    }
  
  return 0;
}

