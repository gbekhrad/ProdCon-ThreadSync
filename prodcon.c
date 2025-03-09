#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include "buffer.h"
#include <time.h>

// Global variables
BUFFER_ITEM buffer[BUFFER_SIZE];

// Synchronization methods/tools
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

// buffer stuff 
int in = 0;   // Next position to write
int out = 0;  // Next position to read


// Function declarations for the producer and consumer threads
void *producer(void *param);
void *consumer(void *param);
int insert_item(BUFFER_ITEM item);
int remove_item(BUFFER_ITEM *item);

// calculate checksum by summing up the bytes in the data array
uint16_t calculate_checksum(uint8_t data[30]) {
    uint16_t checksum = 0;
    for (int i = 0; i < 30; i++) {
        checksum += data[i];
    }
    return checksum;
}

int insert_item(BUFFER_ITEM item) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    
    // Add item and update position
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;  // Circular buffer wrap-around
    
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return 0;
}

int remove_item(BUFFER_ITEM *item) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    
    // Get item and update position
    *item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;  // Circular buffer wrap-around
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return 0;
}

void *producer(void *param) {
    while (1) {
        BUFFER_ITEM item;
        
        // Generate random data for our buffer, 256 from checksum.c
        // 30 given in assignment detail
        for (int i = 0; i < 30; i++) {
            item.data[i] = (rand() % 256);
        }
        
        // Calculate checksum before inserting
        item.cksum = calculate_checksum(item.data);
        
        if (insert_item(item)) {
            fprintf(stderr, "Error: Unable to insert item.\n");
        } else {
            printf("Producer produced item with checksum: %u\n", item.cksum);
        }

        // Random delay time simulate varying work time
        //the usleep function is used to suspend the execution of the calling thread for a specified number of microseconds
        // sleep(1 + (rand() % 3));
        sched_yield();
    }
}

void *consumer(void *param) {
    // uint16_t checksum_result;
    while (1) {
        BUFFER_ITEM item;
        // Random delay
        // sleep(rand() % 3 + 1);
        
        if (remove_item(&item)) {
            fprintf(stderr, "Error: Unable to remove item.\n");
        } else {
            // Check data
            uint16_t checksum_result = calculate_checksum(item.data);
            if (checksum_result != item.cksum) {
                // Use fprintf here because of error handling conventions (source in readme)
                fprintf(stderr, "Error: Checksum mismatch! Expected: %04X, Calculated: %04X\n", item.cksum, checksum_result);
                exit(1);
            }
            printf("Consumer verified item with checksum: %u\n", checksum_result);
        }
        
        // usleep(rand() % 1000000);
        sched_yield();

    }
}

int main(int argc, char *argv[]) {
    // srand(time(NULL));  // Seed the random number generator
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sleep_time> <num_producers> <num_consumers>\n", argv[0]);
        return -1;
    }
    
    /* 1. Get command line arguments argv[1], argv[2], argv[3] */
    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    
    // Error handle that theyre all positive 
    if (sleep_time <= 0 || num_producers <= 0 || num_consumers <= 0) {
        fprintf(stderr, "Error: All arguments must be positive integers.\n");
        return -1;
    }
    
    /* 2. Initialize buffer */
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    // not using malloc anymore bc it causes issues
    /* 3. Create producer thread(s) */
    pthread_t producers[num_producers];
    for (int i = 0; i < num_producers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    
    /* 4. Create consumer thread(s) */
    pthread_t consumers[num_consumers];
    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }
    
    /* 5. Sleep */
    sleep(sleep_time);
    
    /* 6. Exit */
    printf("Exiting program.\n");
    
    // Cleanup synchronization tools
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    return 0;
}
