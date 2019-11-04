/*************************************
 * Lab 5 Ex2
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"
#include <stdio.h>

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {

	//int availableBufferSize = 4096;
	ssize_t count = 0;
	int file_ptr = stream->pointer;
	int temp_buf_pointer = 0;

	if (size > 4096) {
		return -1;
	}

	if (stream->pointer == 0) { //setup buffer in FILE
		ssize_t read_return = read(stream->fd, stream->buf_ptr, 4096);
		if (read_return == 0) {  
			//stream->buffer = buffer;
			//return 1;
			return -1;


		} else if(read_return == -1) {
			return -1;
		}
		
	}

	while (count < nmemb) {
		//printf("POINTER: %d\n", stream->pointer);
		if (stream->buffer[stream->pointer] == '\0') {
			
			return -1;
		}

		char* c = stream->buf_ptr;
		memcpy(ptr, c, 1);

		//ptr = stream->buf_ptr;
		temp_buf_pointer += size;
		file_ptr += size;
		stream->pointer++;
		stream->buf_ptr++;
		count++;

	}

	return count;
}
