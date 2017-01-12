#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

/* The macros of printing functions. */
#ifdef __DEBUG__
#define printLNG(x) printf("%s = %ld\n", #x, (x));
#else
#define printLNG(x) 
#endif
/* End of macros */

#define MILL 1000000

void sleep_us(long sec) {
    printLNG(sec);

    // put time data to timeval_structure{seconds, microseconds}
    struct timeval timeout = { sec / MILL, sec % MILL };

    // call select function to monitor the fd, 
    // and waiting until one or more of the file descriptors 
    // become "ready" for some class of I/O operation.
    select( 0, NULL, NULL, NULL, &timeout);
}

int main(int argc, char *argv[]) 
{
    if( argc != 2 ) {
        fprintf(stderr, "Usage: sleep_us <microseconds>\n");
        exit(1);
    }

    sleep_us(atoi(argv[1]));

    return 0;
}
