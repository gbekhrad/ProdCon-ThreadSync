### Producer-Consumer Problem Simulation via Thread Synchronization 

Gabriella (Gabi) Bekhrad
gabimbek@gmail.com

## Overview/Source Files:
- prodcon.c: C program that uses threads and synchronization tools to simulate a producer-consumer problem.
- buffer.h: Header file that contains the definitions for the buffer and item.
- Checksum.c: C program that contains the checksum function, provided by Professor Springer in the assignment detail. 

## To compile:
gcc prodcon.c -o prodcon

## To run:
./prodcon <delay> <#producer threads> <#consumer threads>

Note:
    <delay> indicates how long to sleep (in seconds) before terminating 
    <#producer threads> indicates number of threads 
    <#consumer threads> indicates the number of consumer threads

##Sample Input/Output: (only a little snippet of output, checked and approved by Professor)
gcc prodcon.c -o prodcon
./prodcon 1 1 1
Consumer verified item with checksum: 3683  
Consumer verified item with checksum: 4107  
Producer produced item with checksum: 3428  
Producer produced item with checksum: 3876  
Consumer verified item with checksum: 3428  
Producer produced item with checksum: 3647  
Consumer verified item with checksum: 3876  
Producer produced item with checksum: 3268  
Producer produced item with checksum: 4417  
Consumer verified item with checksum: 3647  
Consumer verified item with checksum: 3268  
Consumer verified item with checksum: 4417  
Exiting program.  
(because of sched_yield, it is running very fast and 2 lines may be present after the program exits.)  

##Sources:
- https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
- https://dev.to/endeavourmonk/semaphore-in-operating-system-with-its-own-implementation-in-c-19ia
- https://www.geeksforgeeks.org/multithreading-in-c/
- https://support.tetcos.com/support/solutions/articles/14000051594-how-to-use-fprintf-with-stderr-stdout-and-printf-statements-in-netsim-
- https://www.badprog.com/c-type-what-are-uint8-t-uint16-t-uint32-t-and-uint64-t#google_vignette (for understanding)
- https://www.geeksforgeeks.org/c-rand-function/ 
- https://chatgpt.com/ (for debugging purposes) 
- https://stackoverflow.com/questions/68232819/how-to-use-sched-yield-properly 
