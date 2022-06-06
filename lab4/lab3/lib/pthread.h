#include "types.h"
#include "lib.h"
typedef uint32_t pthread_t ;

typedef union  pthread_attr_t{
}pthread_attr_t;

int pthread_create(pthread_t *thread, pthread_attr_t attr, void* (*start_routine)(void*),  void* args);
