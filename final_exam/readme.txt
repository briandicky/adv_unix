#Advanced UNIX Programming Final Exam

##question 2, user-defined sleep_us function
This function, sleep_us, which is similar to sleep function, however, it waits for a specified number of microseconds.
I use the timeval structure to store the waiting time, which is inputted from user manually.
Then using the select function to monitor the file descriptor, and waiting until one or more of (here is one) the file descriptors become "ready" for some class of I/O operation.

At first, you need to compile the program by using the commands below.
```
make
```
or
```
make clean all
```

Then, use the time command to measure whether the sleep_us function returns in the precise time.
```
time ./sleep_us <microseconds>
```

##Alarm, implemented by a single timer
Use a single timer to implement a set of functions that
