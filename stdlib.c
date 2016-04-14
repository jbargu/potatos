#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

uint8_t *free_mem = (uint8_t*) 0xA00000;

// Please do feel amazed at this ingenious code. Probably one of the top 3
// I've ever written. Such efficiency. No leaks guaranteed.
void *malloc(size_t size) {
    void *curr_memory = free_mem;
    free_mem += size;
    return curr_memory;
}

// Convert positive integer to string.
char *itoa(int n) {
      char *buf = (char *)malloc(5);

      if (!n){
	  buf[0] = '0';
	  buf[1] = '\0';
	  return buf;
      }
      int i;
      for (i = 0; n > 0; i++) {
	    buf[i] = (n % 10) + '0';
	    n /= 10;
      }
      buf[i] = '\0';
      reverse(buf);
      return buf;
}
