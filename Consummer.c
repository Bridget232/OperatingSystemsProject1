 /* Consummer Code (Consummer.c */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CONSUMABLE_ITEMS 2 // Maximum number of items that can be consumed by each consumer
#define BUFFER_SIZE 2 // The size of the shared buffer

sem_t available_slots, occupied_slots; // Semaphores for available slots and occupied slots
int buffer_index_produce = 0; // Index for inserting items into the buffer
int buffer_index_consume = 0; // Index for removing items from the buffer

int buffer[BUFFER_SIZE]; // Shared buffer for items
pthread_mutex_t buffer_mutex; // Mutex to protect the shared buffer

// Consumer thread function
void* consumer(void* consumer_id) {
    int item, i;
    for (i = 0; i < MAX_CONSUMABLE_ITEMS; i++) {
        sem_wait(&occupied_slots); // Wait for an occupied slot (i.e., an item to consume)
        pthread_mutex_lock(&buffer_mutex); // Acquire lock to safely access the buffer
        
        item = buffer[buffer_index_consume]; // Retrieve item from buffer
        printf("Consumer %d: removed item %d from index %d\n", *((int*)consumer_id), item, buffer_index_consume);
        
        buffer_index_consume = (buffer_index_consume + 1) % BUFFER_SIZE; // Update buffer index for consuming
        
        pthread_mutex_unlock(&buffer_mutex); // Release lock
        sem_post(&available_slots); // Signal that a slot is available in the buffer
    }
    return NULL;
}

int main() {
    pthread_t consumer_threads[2]; // Array for storing consumer thread handles
    int consumer_ids[2] = { 1, 2 }; // IDs for each consumer

    // Initialize the mutex and semaphores
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&available_slots, 0, BUFFER_SIZE); // Initially, all buffer slots are available
    sem_init(&occupied_slots, 0, 0); // Initially, no buffer slots are occupied

    // Create consumer threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, (void*)&consumer_ids[i]);
    }

    // Wait for all consumer threads to complete
    for (int i = 0; i < 2; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Destroy the mutex and semaphores
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&available_slots);
    sem_destroy(&occupied_slots);

    return 0;
}

