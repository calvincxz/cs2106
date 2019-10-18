/*************************************
 * Lab 3 Exercise 3
 * Name: Calvin
 * Student No: A0190624H
 * Lab Group: 4
 *************************************/
 
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;
sem_t maxCarInSegments; 
pthread_mutex_t mutexes[100];
int queue[100];
pthread_mutex_t noReader[100];
pthread_mutex_t noWriter[100];

void initialise(int num_of_cars, int num_of_segments)
{
    //pthread_mutex_t mutexes[num_of_segments];
    //pthread_mutex_init(&(lock->roomEmpty), NULL);
    //forks = [ Semaphore (1) for i in range (5)]
    for (int i = 0; i < num_of_segments; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
        pthread_mutex_init(&noReader[i], NULL);
        pthread_mutex_init(&noWriter[i], NULL);
    }
    // Now you can use them safely.

    sem_init(&maxCarInSegments, 0, num_of_segments - 1); 
}

void cleanup()
{
    sem_destroy(&maxCarInSegments); 
    for (int i = 0; i < num_of_segments; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
}

void* car(void* car2)
{
    //This function modeles a thread 
    //A car: 
    //   -should call enter_roundabout (...)
    //   -followed by some calls to move_to_next_segment (...)
    //   -finally call exit_roundabout (...)
    car_struct *car = (car_struct*) car2;

    sem_wait(&maxCarInSegments);
    pthread_mutex_lock(&mutexes[car->entry_seg]);
    pthread_mutex_lock(&noReader[car->current_seg]);
    	pthread_mutex_lock(&noWriter[car->current_seg]);
    pthread_mutex_unlock(&noReader[car->current_seg]);
    enter_roundabout(car);

    pthread_mutex_unlock(&noWriter[car->current_seg]);

    //pthread_mutex_unlock(&segments[car->entry_seg]);
    
    while (car->current_seg != car->exit_seg) {
    	queue[car->current_seg]++;
    	if (queue[car->current_seg] == 1) {
    		 pthread_mutex_lock(&noReader[car->current_seg]);
    	}
    	pthread_mutex_lock(&noWriter[car->current_seg]);
        pthread_mutex_lock(&mutexes[NEXT(car->current_seg, num_of_segments)]); //priority
        move_to_next_segment(car);
        pthread_mutex_unlock(&noWriter[PREV(car->current_seg, num_of_segments)]);

    	queue[car->current_seg]--;
    	if (queue[car->current_seg] == 0) {
    		 pthread_mutex_unlock(&noReader[PREV(car->current_seg, num_of_segments)]);
    	}
        pthread_mutex_unlock(&mutexes[PREV(car->current_seg, num_of_segments)]);
    }

    //pthread_mutex_unlock(&mutexes[car->current_seg]);
	exit_roundabout(car);
    pthread_mutex_unlock(&mutexes[car->current_seg]);
    sem_post(&maxCarInSegments);

    
    pthread_exit(NULL);
}


