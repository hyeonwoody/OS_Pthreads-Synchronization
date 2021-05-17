#include <pthread.h>

#define DEBUG 0
#define DEBUG1 0
#define DEBUG2 0
#define TOGGLE 0

typedef struct SSU_Sem {
#if DEBUG
    int address;
#endif
    int counter;

    pthread_cond_t cond;
    pthread_mutex_t lock;
} SSU_Sem;

void SSU_Sem_init(SSU_Sem *, int);
void SSU_Sem_destroy(SSU_Sem *);
void SSU_Sem_up(SSU_Sem *);
void SSU_Sem_down(SSU_Sem *);
void SSU_Sem_wait (SSU_Sem *s);
void SSU_Sem_post (SSU_Sem *s);
int SSU_Sem_create(SSU_Sem *, const pthread_attr_t *attr, void *(fp)(void*), void *);
int SSU_Sem_join (SSU_Sem, void **);
