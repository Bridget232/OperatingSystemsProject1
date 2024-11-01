

 /* Producer Code (Producer.c) */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRODUCTION_COUNT 2 // maximum number of producible items
#define BUFFER_CAPACITY 2 // the size of the buffer at any instance

extern sem_t empty_slots, filled_slots; // semaphores to keep track of empty and filled slots
extern int buffer_insert_index; // index to insert items in buffer
extern int buffer[]; // buffer to hold produced items
extern pthread_mutex_t buffer_mutex; // mutex to protect buffer access

// Function executed by producer threads
void* producer_function(void* producer_number) {
    int produced_item, i;
    for (i = 0; i < MAX_PRODUCTION_COUNT; i++) {
        produced_item = rand(); // produce an item (random number)
        
        sem_wait(&empty_slots); // wait until there's an empty slot in the buffer
        pthread_mutex_lock(&buffer_mutex); // acquire lock to insert item in buffer
        
        buffer[buffer_insert_index] = produced_item; // insert the item in buffer
        printf("Producer %d: Inserted item %d at index %d\n", *((int*)producer_number), buffer[buffer_insert_index], buffer_insert_index);
        buffer_insert_index = (buffer_insert_index + 1) % BUFFER_CAPACITY; // update insert index
        
        pthread_mutex_unlock(&buffer_mutex); // release lock after inserting item
        sem_post(&filled_slots); // signal that a new item has been added to buffer
    }
    return NULL;
}

/* Consumer Code (consumer.c) */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CONSUMPTION_COUNT 2 // maximum number of consumable items
#define BUFFER_CAPACITY 2 // the size of the buffer at any instance

extern sem_t empty_slots, filled_slots; // semaphores to keep track of empty and filled slots
extern int buffer_remove_index; // index to remove items from buffer
extern int buffer[]; // buffer to hold produced items
extern pthread_mutex_t buffer_mutex; // mutex to protect buffer access

// Consumer thread function
void* consumer_function(void* consumer_id) {
    int consumed_item, i;
    for (i = 0; i < MAX_CONSUMPTION_COUNT; i++) {
        sem_wait(&filled_slots); // wait until there's an item to consume in the buffer
        pthread_mutex_lock(&buffer_mutex); // acquire lock to remove item from buffer

        consumed_item = buffer[buffer_remove_index]; // retrieve item from buffer
        printf("Consumer %d: Removed item %d from index %d\n", *((int*)consumer_id), consumed_item, buffer_remove_index);
        buffer_remove_index = (buffer_remove_index + 1) % BUFFER_CAPACITY; // update remove index

        pthread_mutex_unlock(&buffer_mutex); // release lock after consuming item
        sem_post(&empty_slots); // signal that a slot is available in the buffer
    }
    return NULL;
}

/* Main Program (main.c) */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_CAPACITY 2 // the size of the buffer at any instance

sem_t empty_slots, filled_slots; // semaphores to keep track of empty and filled slots
int buffer_insert_index = 0; // index to insert items in buffer
int buffer_remove_index = 0; // index to remove items from buffer

int buffer[BUFFER_CAPACITY]; // buffer to hold produced items
pthread_mutex_t buffer_mutex; // mutex to protect buffer access

extern void* producer_function(void* producer_number);
extern void* consumer_function(void* consumer_id);

int main() {
    pthread_t producers[2]; // array to store producer thread IDs
    pthread_t consumers[2]; // array to store consumer thread IDs
    pthread_mutex_init(&buffer_mutex, NULL); // initialize mutex
    sem_init(&empty_slots, 0, BUFFER_CAPACITY); // initialize semaphore for empty slots
    sem_init(&filled_slots, 0, 0); // initialize semaphore for filled slots

    int producer_ids[2] = { 1, 2 }; // producer IDs
    int consumer_ids[2] = { 1, 2 }; // consumer IDs

    // Create producer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&producers[i], NULL, producer_function, (void*)&producer_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&consumers[i], NULL, consumer_function, (void*)&consumer_ids[i]);
    }

    // Wait for producer threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for consumer threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Clean up resources
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);

    return 0;
}
