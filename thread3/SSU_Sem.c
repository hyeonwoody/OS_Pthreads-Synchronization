#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "SSU_Sem.h"



void SSU_Sem_init(SSU_Sem *s, int value) {
    s->counter=value;
#if DEBUG   
    s->address=(int)s;
#endif
    pthread_cond_init(&s->cond, NULL);
    pthread_mutex_init(&s->lock, NULL);
#if DEBUG 
    printf ("\n%d is assigned\n", s->address); 
#endif
}



void SSU_Sem_down(SSU_Sem *s) {
    
    pthread_mutex_lock (&s->lock);
#if DEBUG 
    printf ("\n%d is downing\n", s->address);
#endif
    s->counter--;
    if (s->counter<0){
        //pthread_mutex_unlock (&s->lock); 
         if (DEBUG) printf ("\nbefore wait\n");
        pthread_cond_wait(&s->cond, &s->lock);
        //pthread_mutex_lock (&s->lock);
        if (DEBUG)  printf("after signal\n");
    }
#if DEBUG    
    printf("fine"); 
#endif
    pthread_mutex_unlock (&s->lock);
}

void SSU_Sem_up(SSU_Sem *s) {
#if DEBUG 
    printf ("\n%d is upping\n", s->address);
#endif
    pthread_mutex_lock (&s->lock);
    s->counter++;
    if (DEBUG)  printf("before signal\n");
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock (&s->lock);
}

void SSU_Sem_destroy(SSU_Sem *s) {
    pthread_cond_destroy(&s->cond);
    pthread_mutex_destroy (&s->lock);
}

 /* void SSU_Sem_wait (SSU_Sem *s){
    pthread_mutex_lock (&s->lock);
    while (s->counter <=0){
        pthread_cond_wait(&s->cond, &s->lock);
    }
    s->counter--;
    pthread_mutex_unlock (&s->lock);
}

void SSU_Sem_post (SSU_Sem *s){
    pthread_mutex_lock (&s->lock);
    s->counter++;
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock (&s->lock);
}



int SSU_Sem_create(SSU_Sem *s, const pthread_attr_t *attr, void *(fp)(void*), void *arg) {
    fp(arg);
#if DEBUG1
    printf("done");
#endif
}

int SSU_Sem_join (SSU_Sem s, void **value_ptr){

}  */