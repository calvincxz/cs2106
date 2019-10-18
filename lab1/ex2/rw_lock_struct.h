/*************************************
 * Lab 3 Exercise 2
 * Name: Calvin
 * Student No: A0190624H
 * Lab Group: 4
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t roomEmpty;
  pthread_mutex_t turnstile;
  int reader_count;
  int writer_count;
} rw_lock;

