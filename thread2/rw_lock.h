#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

struct rw_lock
{
    pthread_mutex_t lock;
    pthread_mutex_t readerlock;
    pthread_mutex_t writerlock;

	pthread_cond_t read;
	pthread_cond_t write;

    int readers;
    int writers;
    int waiters;
};

void init_rwlock(struct rw_lock * rw);
void r_lock(struct rw_lock * rw);
void r_unlock(struct rw_lock * rw);
void w_lock(struct rw_lock * rw);
void w_unlock(struct rw_lock * rw);
long *max_element(long* start, long* end);
long *min_element(long* start, long* end);
