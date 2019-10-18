/*************************************
 * Lab 4 Exercise N
 * Name: 
 * Student No:
 * Lab Group:
 *************************************/

// You can modify anything in this file. 
// Unless otherwise stated, a line of code being present in this template 
//  does not imply that it is correct/necessary! 
// You can also add any global or local variables you need (e.g. to implement your page replacement algorithm).

#include <signal.h>
#include <stdio.h>

#include "api.h"

void os_run(int initial_num_pages, page_table *pg_table){
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    int frame_size = 1<<FRAME_BITS;
    
    // create the pages in the disk first, because every page must be backed by the disk for this lab
    for (int i=0; i!=initial_num_pages; ++i) {
        disk_create(i);
    }
    
    int frame_page = -1;
    int circularQueue[frame_size];
    int next_victim = 0;

    for (int i = 0; i < frame_size; i++) {
    	circularQueue[i] = -1;
    }
    //disk_read(0, 1);
    while (1) {
        siginfo_t info;

        sigwaitinfo(&signals, &info);
        
        // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
        int requested_page = info.si_value.sival_int;
        //disk_read(0, requested_page);
        if (requested_page == -1) break;
        
        
        // process the signal, and update the page table as necessary
    		
		while (circularQueue[next_victim] != -1) { // frame not empty
			if (pg_table->entries[circularQueue[next_victim]].referenced == 0) {
				pg_table->entries[circularQueue[next_victim]].valid = 0;
				break;
			} else {
				pg_table->entries[circularQueue[next_victim]].referenced = 0;
			}
			next_victim = (next_victim + 1) % frame_size;
		}
		disk_read(next_victim, requested_page);
		pg_table->entries[requested_page].valid = 1;
		pg_table->entries[requested_page].referenced = 0;
		pg_table->entries[requested_page].frame_index = next_victim;
		circularQueue[next_victim] = requested_page;
		next_victim = (next_victim + 1) % frame_size;
 
        
        // tell the MMU that we are done updating the page table
        union sigval reply_value;
        reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)
        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
}
    