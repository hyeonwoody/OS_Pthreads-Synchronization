#include "rw_lock.h" //writer main

void init_rwlock(struct rw_lock * rw)
{
  while (pthread_mutex_init(&rw->lock, NULL)!=0){}
  while (pthread_mutex_init(&rw->writerlock, NULL)!=0){}
  while (pthread_cond_init(&rw->write, NULL)!=0){} 
  while (pthread_cond_init(&rw->read, NULL)!=0){}
  
  rw->writers=0;
  rw->readers=0;
  rw->waiters=0;
}

void r_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  
  while(rw->writers){pthread_cond_wait(&rw->read, &rw->lock);}
  //wait until no more writer is waiting to get the lock.
  while(rw->waiters){pthread_cond_wait(&rw->read, &rw->lock);} 
  rw->readers++;
  pthread_mutex_unlock(&rw->lock);
}

void r_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
	rw->readers--;
	if (rw->writers == 0){
		pthread_cond_signal(&rw->write);
  }
  else {pthread_cond_signal(&rw->read);}
	pthread_mutex_unlock(&rw->lock);
}

void w_lock(struct rw_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
	rw->waiters++;
  while(rw->readers|| rw->writers) {
		pthread_cond_wait(&rw->write, &rw->lock); 
	}
  //no writers waiting
  rw->waiters--; rw->writers++;
	
	pthread_mutex_unlock(&rw->lock); 

}

void w_unlock(struct rw_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
	rw->writers--;
	
  if (rw->writers<=0){
    pthread_cond_signal(&rw->write);
    if (rw->waiters<=0){
    pthread_cond_broadcast(&rw->read);
    }
  } 
	pthread_mutex_unlock(&rw->lock);
}
