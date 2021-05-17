#include "rw_lock.h" //writer main
void init_rwlock(struct rw_lock * rw)
{
  while (pthread_mutex_init(&rw->lock, NULL)!=0){}
  while (pthread_mutex_init(&rw->readerlock, NULL)!=0){}
  while (pthread_mutex_init(&rw->writerlock, NULL)!=0){}
  while (pthread_cond_init(&rw->write, NULL)!=0){} 
  while (pthread_cond_init(&rw->read, NULL)!=0){}

  rw->writers=0;
  rw->readers=0;
  rw->waiters=0;
}

void r_lock(struct rw_lock * rw)
{
  pthread_mutex_lock(&rw->readerlock);
  rw->readers++;
  pthread_mutex_unlock(&rw->readerlock); 
 
}

void r_unlock(struct rw_lock * rw)
{

  pthread_mutex_lock(&rw->readerlock);
	rw->readers--;
      pthread_cond_signal(&rw->write);
	pthread_mutex_unlock(&rw->readerlock);
  
}

void w_lock(struct rw_lock * rw)
{
  pthread_mutex_lock(&rw->writerlock);  
  rw->waiters++;
  while (rw->readers!=0 || rw->writers!=0)
    pthread_cond_wait(&rw->write, &rw->writerlock);
  rw->writers++;
  pthread_mutex_unlock(&rw->writerlock);
}

void w_unlock(struct rw_lock * rw)
{

  pthread_mutex_lock(&rw->writerlock);
  
  rw->writers--;
  pthread_cond_signal(&rw->read);
  if (rw->readers == 0){
		while(pthread_cond_broadcast(&rw->write)!=0){}
  }
  if (rw->waiters == 0){
		while(pthread_cond_broadcast(&rw->write)!=0){}
  }
  
  pthread_mutex_unlock(&rw->writerlock); 
  
}
