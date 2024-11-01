# OperatingSystemsProject1
Producer-Consumer Program using POSIX Threads, Semaphores, and Mutexes

Overview

This project is an implementation of the classic Producer-Consumer problem, which is a fundamental problem in computer science that involves synchronizing multiple producer and consumer threads accessing a shared buffer. The implementation uses POSIX threads, semaphores, and mutexes to ensure correct synchronization and prevent race conditions.

Description

The Producer-Consumer problem involves two types of threads:

Producer Threads: Responsible for generating data and adding it to a shared buffer.

Consumer Threads: Responsible for consuming data from the shared buffer.

This program uses semaphores to track the availability of buffer slots and mutexes to ensure only one thread can modify the buffer at a time. This synchronization mechanism ensures that producers do not add items to a full buffer, and consumers do not attempt to remove items from an empty buffer.

Features

Multiple Producer and Consumer Threads: The program creates two producer threads and two consumer threads.

Fixed Size Buffer: A circular buffer of fixed size (capacity = 2) is shared between the producer and consumer threads.

Synchronization Mechanisms: Semaphores and mutex locks are used to synchronize the buffer access among threads.

Components

Buffer: A shared array that stores the items produced by the producer threads. The buffer has a fixed capacity of 2.

Semaphores:

empty_slots: Tracks the available slots in the buffer.

filled_slots: Tracks the number of items available in the buffer for consumption.

Mutex: A mutex (buffer_mutex) is used to protect access to the shared buffer and prevent data corruption by ensuring only one thread accesses the buffer at a time.

Flow of Execution

Producer Threads: Each producer generates a random item and waits until an empty slot is available in the buffer. Once available, the item is inserted into the buffer, and the producer signals the consumers by incrementing the filled_slots semaphore.

Consumer Threads: Each consumer waits until there is an item in the buffer to consume. Once an item is available, it is removed from the buffer, and the consumer signals the producers by incrementing the empty_slots semaphore.

Semaphores and Mutex Locks: The semaphores (empty_slots and filled_slots) ensure that the buffer does not overflow or underflow. The mutex (buffer_mutex) ensures mutual exclusion when accessing the buffer.

Compilation and Execution

To compile the code, use the following command:

gcc -o producer_consumer producer_consumer.c -lpthread

To execute the program:

./producer_consumer

Example Output

Producer 1: Inserted item 12345 at index 0
Producer 2: Inserted item 67890 at index 1
Consumer 1: Removed item 12345 from index 0
Consumer 2: Removed item 67890 from index 1

The output shows the sequence of operations performed by the producer and consumer threads as they interact with the shared buffer.

Synchronization Details

The empty_slots semaphore is initialized with the buffer capacity (2) to indicate that all slots are initially empty.

The filled_slots semaphore is initialized to 0, as initially no items are in the buffer.

Producer Threads:

Before adding an item, a producer decrements the empty_slots semaphore to ensure there is space in the buffer.

After adding an item, the producer increments the filled_slots semaphore to signal that an item is available.

Consumer Threads:

Before removing an item, a consumer decrements the filled_slots semaphore to ensure there is an item to consume.

After removing an item, the consumer increments the empty_slots semaphore to indicate that a slot is available.

Summary

This program provides an implementation of the Producer-Consumer problem using threads, semaphores, and mutexes to illustrate key concepts in concurrency and synchronization. The use of POSIX threads and synchronization primitives ensures the correct and safe interaction of producer and consumer threads with the shared buffer.

License

This project is open-source and distributed under the MIT License.
