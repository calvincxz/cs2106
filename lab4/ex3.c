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
    sigaddset(&signals, SIGUSR2);
    int frame_size = 1<<FRAME_BITS;
    int max_page_num = (1<<PAGE_BITS) - 1;
    int pageCreated[max_page_num];
    
    for (int i = 0; i < max_page_num; i++) {
        pageCreated[i] = 0;
    }

    // create the pages in the disk first, because every page must be backed by the disk for this lab
    for (int i=0; i!=initial_num_pages; ++i) {
        disk_create(i);
        pageCreated[i] = 1;
    }
    
    //int frame_page = -1;
    int circularQueue[frame_size];
    int next_victim = 0;
    //int new_page_num = initial_num_pages;

    for (int i = 0; i < frame_size; i++) {
    	circularQueue[i] = -1;
    }
    while (1) {
        siginfo_t info;
        sigwaitinfo(&signals, &info);
        union sigval reply_value;
        if (info.si_signo == SIGUSR1) {
            // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
            int requested_page = info.si_value.sival_int;
            //disk_read(0, requested_page);
            if (requested_page == -1) break;
            
            // process the signal, and update the page table as necessary
            if (requested_page >= 0 && pageCreated[requested_page] == 1) { // page exists
                int victim_page = circularQueue[next_victim];
        		while (victim_page != -1) { // frame not empty
                    if (pg_table->entries[victim_page].valid == 0) {
                        break;
                    }

                    if (pg_table->entries[victim_page].referenced == 0) {
                        pg_table->entries[victim_page].valid = 0;

                        if (pg_table->entries[victim_page].dirty == 1) {
                            disk_write(next_victim, victim_page);
                        }
                        break;

        			} else { // referenced == 1
        				pg_table->entries[victim_page].referenced = 0;
                        next_victim = (next_victim + 1) % frame_size;
                        victim_page = circularQueue[next_victim];
        			}

                }

                disk_read(next_victim, requested_page);
                pg_table->entries[requested_page].valid = 1;
                pg_table->entries[requested_page].referenced = 0;
                pg_table->entries[requested_page].dirty = 0;
                pg_table->entries[requested_page].frame_index = next_victim;
                circularQueue[next_victim] = requested_page;
                next_victim = (next_victim + 1) % frame_size;
            	reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)

            } else {
              reply_value.sival_int = 1;
            }

            // tell the MMU that we are done updating the page table

        } else { //sigusr2
            int requested_page = info.si_value.sival_int;
            
            if (requested_page == -1) { //mmap
                for (int i = 0; i <= max_page_num; i++) {
                    if (pageCreated[i] == 0) { //find a non-existing page
                        disk_create(i);
                        pageCreated[i] = 1;
                        reply_value.sival_int = i;
                        break;
                    } 
                }

            } else { //munmap
                
                if (pageCreated[requested_page] == 0) {
                    reply_value.sival_int = 1;
                    sigqueue(info.si_pid, SIGCONT, reply_value);
                    continue;
                }

                if (pg_table->entries[requested_page].valid == 1) {
                    pg_table->entries[requested_page].valid = 0;
                } 

                reply_value.sival_int = 0;
                disk_delete(requested_page);
                pageCreated[requested_page] = 0;
            }

        }

        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
}

