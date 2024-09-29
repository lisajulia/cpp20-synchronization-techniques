#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <chrono>
#include <random>

// Semaphore that controls access to the limited number of database connections
std::counting_semaphore<3> connection_semaphore(3);  // Max. 3 concurrent connections, start with 3 possible connections

std::random_device rd;   // Non-deterministic random number generator
std::mt19937 gen(rd());  // Seed the generator
std::uniform_int_distribution<> dist_access(1, 3);  // Distribution for random wait time
std::uniform_int_distribution<> dist_acquire(1, 10);  // Distribution for random wait time

// Simulated database connection function
void access_database_critical(int thread_id) {
    // Wait for a connection to become available
    connection_semaphore.acquire();  // Decrease semaphore count
    int waiting_time = dist_access(gen);

    // Simulate database access
    std::cout << "Critial operation: Thread " << thread_id << " is accessing the database for " << waiting_time << " seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));  // Simulate time to access the database
    std::cout << "Critial operation: Thread " << thread_id << " has finished accessing the database.\n";

    // Release the connection
    connection_semaphore.release();  // Increase semaphore count, signaling that a connection is free
}

void access_database_non_critical(int thread_id) {
    int acquire_time = dist_acquire(gen);
    // Wait for a connection to become available
    bool acquired = connection_semaphore.try_acquire_for(std::chrono::seconds(acquire_time));  // Decrease semaphore count
    if (acquired) {
        int waiting_time = dist_access(gen);

        // Simulate database access
        std::cout << "Non-critical operation: Thread " << thread_id << " is accessing the database for " << waiting_time << " seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(waiting_time));  // Simulate time to access the database
        std::cout << "Non-critical operation: Thread " << thread_id << " has finished accessing the database.\n";

        // Release the connection
        connection_semaphore.release();  // Increase semaphore count, signaling that a connection is free
    } else {
        std::cout << "Non-critical operation: Thread " << thread_id << " could not access the database.\n";
    }
}

int main() {
    const int num_threads = 20;  // Total number of threads requesting access

    std::vector<std::thread> threads;

    // Launch 10 threads that all need to access the database
    int i = 0;
    while (i < num_threads) {
        threads.emplace_back(access_database_critical, i++);
        threads.emplace_back(access_database_non_critical, i++);
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All threads have finished their work.\n";
    return 0;
}

