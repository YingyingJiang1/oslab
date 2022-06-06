#include "pthread.h"
int pthread_create(pthread_t *thread, pthread_attr_t attr, void* (*start_routine)(void*),  void* args)
{
   int ret =  syscall(SYS_PTHREAD_CREATE, thread, attr, start_routine, args);
    return ret;
}