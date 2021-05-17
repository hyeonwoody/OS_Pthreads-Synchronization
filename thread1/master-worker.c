#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

int item_to_produce, curr_buf_size;
int total_items, max_buf_size, num_workers, num_masters;

int *buffer;

pthread_mutex_t master,worker;

pthread_mutex_t masterfull,masterproduce;
pthread_mutex_t workernotfull,workerconsume;

pthread_cond_t full, empty;





void print_buffer(){
  for (int z=0; z<max_buf_size; z++){
    if (buffer[z]>=0){
      printf("[%d] ", buffer[z]);
    }
    if (buffer[z]==-1){
      printf("[N] ");
    }
  }
  printf ("curr : %d,", curr_buf_size);
  printf("\n");
}


void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
  { 
    while (pthread_mutex_lock(&master)!=0){}
    
    if (item_to_produce >= total_items) { //done producing items
      while (pthread_cond_broadcast(&full)!=0){} //remainder that cannot be consumed after buffer fullfilled
      while (pthread_mutex_unlock(&master)!=0){} 
      break; //quit producing
    }

    while (curr_buf_size==max_buf_size){

      while (pthread_cond_signal(&full)!=0){}; 
      while (pthread_mutex_lock(&masterfull)!=0){}
      while (pthread_cond_wait(&empty, &masterfull)!=0){} //pause producing until buffer is empty
      while (pthread_mutex_unlock(&masterfull)!=0){}
    }

    

    while (pthread_mutex_lock(&masterproduce)!=0){}
      buffer[curr_buf_size++] = item_to_produce; 
      print_produced(item_to_produce++, thread_id);
      
    while(pthread_mutex_unlock(&masterproduce)!=0){}
    //print_buffer();
    
    while(pthread_mutex_unlock(&master)!=0){}
  }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item
void *consume_requests_loop(void *data){
  int thread_id = *((int *)data);

  while(1)
  {
    while(pthread_mutex_lock(&worker)!=0){}

    if (item_to_produce >= total_items && curr_buf_size==0) { //nothing to produce, nothing to consume
      while(pthread_mutex_unlock(&worker)!=0){}
      break;//quit consuming 
    }

    while (curr_buf_size==0){ //buffer is empty
      while(pthread_mutex_lock(&workernotfull)!=0){}
      while(pthread_cond_wait(&full, &workernotfull)!=0){} //wait until buffer is full
      while(pthread_mutex_unlock(&workernotfull)!=0){}
    }
    
    while(pthread_mutex_lock(&workerconsume)!=0){} //consuming starts
    print_consumed(buffer[curr_buf_size-1], thread_id);
    buffer[--curr_buf_size]=-1; //remove
    //print_buffer();
    
    if(curr_buf_size == 0) { 
     while(pthread_cond_signal(&empty)!=0){}
     }
    while(pthread_mutex_unlock(&workerconsume)!=0){} //consuming ends
    while(pthread_mutex_unlock(&worker)!=0){}
  }
  return 0;
}


int main(int argc, char *argv[])
{
  int *master_thread_id;
  int *worker_thread_id;
  pthread_t *master_thread;
  pthread_t *worker_thread;
  item_to_produce = 0;
  curr_buf_size = 0;

  
  int i;
  
   if (argc < 5) {
    //printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    total_items = atoi(argv[1]); //M
    max_buf_size = atoi(argv[2]); //N
    num_workers = atoi(argv[3]); //C
    num_masters = atoi(argv[4]); //P    
  }
  /* while (pthread_mutex_init(&master, NULL)!=0){}
  while (pthread_mutex_init(&worker, NULL)!=0){}
  while (pthread_mutex_init(&masterfull, NULL)!=0){}  
  while (pthread_mutex_init(&masterproduce, NULL)!=0){} 
  while (pthread_mutex_init(&workernotfull, NULL)!=0){}  
  while (pthread_mutex_init(&workerconsume, NULL)!=0){} 
  while (pthread_cond_init(&full, NULL)!=0){}
  while (pthread_cond_init(&empty, NULL)!=0){} */

  buffer = (int *)malloc (sizeof(int) * max_buf_size);

  //initialize master producer threads
  master_thread_id = (int *)malloc(sizeof(int) * num_masters);
  master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++)
    master_thread_id[i] = i;
  
  //initialize worker consumer threads
  worker_thread_id = (int *)malloc(sizeof(int) * num_workers);
  worker_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_workers);
  for (i = 0; i < num_workers; i++)
    worker_thread_id[i] = i;

  //create master producer threads
  for (i = 0; i < num_masters; i++){
    pthread_create(&master_thread[i], NULL, generate_requests_loop, (void *)&master_thread_id[i]);
    printf("master %d created\n", i);
  }
  //create worker consumer threads
  for (i=0; i<num_workers; i++){
    pthread_create(&worker_thread[i], NULL, consume_requests_loop, (void *)&worker_thread_id[i]);
    printf("worker %d created\n", i);
  }
  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
    {
      pthread_join(master_thread[i], NULL);
      printf("master %d joined\n", i);
    }
  for (i = 0; i < num_workers; i++)
  {
      pthread_join(worker_thread[i], NULL);
      printf("worker %d joined\n", i);
  }

  
  /*----Deallocating Buffers---------------------*/
  free(buffer);
  free(master_thread_id);
  free(master_thread);
  free(worker_thread_id);
  free(worker_thread);
  pthread_mutex_destroy(&master);
  pthread_mutex_destroy(&worker);
  pthread_mutex_destroy(&masterfull);
  pthread_mutex_destroy(&masterproduce);
  pthread_mutex_destroy(&workernotfull);
  pthread_mutex_destroy(&workerconsume);
  pthread_cond_destroy(&full);
  pthread_cond_destroy(&empty);/**/
  return 0;
}
