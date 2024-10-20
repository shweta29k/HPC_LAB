#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> 

#define BUFFER_SIZE 10 
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 10

int buffer[BUFFER_SIZE]; 
int in = 0; // Index for producers
int out = 0; // Index for consumers
pthread_mutex_t mtx; // Mutex for synchronization
pthread_cond_t not_full; // Condition variable for buffer not full
pthread_cond_t not_empty; // Condition variable for buffer not empty

// Producer function
void* producer(void* arg) {
    intptr_t id = (intptr_t)arg; // Use intptr_t for casting
    free(arg); // Free the allocated memory for thread ID

    for (int i = 0; i < NUM_ITEMS; ++i) {
        pthread_mutex_lock(&mtx);

        // Wait if buffer is full
        while ((in + 1) % BUFFER_SIZE == out) {
            pthread_cond_wait(&not_full, &mtx);
        }

        // Produce an item
        int item = id * 10 + i;
        buffer[in] = item;
        printf("Producer %ld produced %d\n", (long)id, item); // Use %ld for intptr_t

        // Update the index
        in = (in + 1) % BUFFER_SIZE;

        // Notify consumers
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    intptr_t id = (intptr_t)arg; // Use intptr_t for casting
    free(arg); // Free the allocated memory for thread ID

    for (int i = 0; i < NUM_ITEMS; ++i) {
        pthread_mutex_lock(&mtx);

        // Wait if buffer is empty
        while (in == out) {
            pthread_cond_wait(&not_empty, &mtx);
        }

        // Consume an item
        int item = buffer[out];
        printf("Consumer %ld consumed %d\n", (long)id, item); // Use %ld for intptr_t

        // Update the index
        out = (out + 1) % BUFFER_SIZE;

        // Notify producers
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mtx);
    }

    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    // Initialize mutex and condition variables
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        intptr_t* id = (intptr_t*)malloc(sizeof(intptr_t)); // Correctly cast malloc return value
        *id = i;
        pthread_create(&producers[i], NULL, producer, (void*)id);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        intptr_t* id = (intptr_t*)malloc(sizeof(intptr_t)); // Correctly cast malloc return value
        *id = i;
        pthread_create(&consumers[i], NULL, consumer, (void*)id);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumers[i], NULL);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}

