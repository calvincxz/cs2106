
/*************************************
 * Lab 3 Exercise 1
 * Name: Calvin
 * Student No: A0190624H
 * Lab Group: 4
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL); //initialised and unlocked
  pthread_mutex_init(&(lock->roomEmpty), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->roomEmpty));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->roomEmpty));
}

void reader_acquire(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  if (lock->reader_count == 0) {
  	pthread_mutex_lock(&(lock->roomEmpty));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  pthread_mutex_lock(&(lock->mutex));
  if (lock->reader_count == 1) {
  	pthread_mutex_unlock(&(lock->roomEmpty));
  }
  lock->reader_count--;
  pthread_mutex_unlock(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
  pthread_mutex_destroy(&(lock->mutex));
  pthread_mutex_destroy(&(lock->roomEmpty));
}
