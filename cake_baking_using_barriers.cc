#include <iostream>
#include <thread>
#include <barrier>
#include <array>
#include <random>
#include <string>

// Random number generator setup
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> shortTime(1, 7);
std::uniform_int_distribution<> longTime(5, 14);

// Fixed-size array for random time distributions
std::array<std::uniform_int_distribution<>, 2> dists{shortTime, longTime};

// People in each crew
const int crewSize = 2;

// Function to map crewId to a name for output clarity
std::string getCrewName(int crewId) {
    return crewId == 0 ? "Crew A" : "Crew B";
}

void bakingTask(int crewId, int id, std::barrier<>& barrier) {
    std::string crewName = getCrewName(crewId);

    // Phase 1: Prepare Ingredients
    auto seconds = dists[crewId](gen);
    std::cout << crewName << ", Baker " << id << " is preparing ingredients for " << seconds << " seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds)); // Simulate task time

    std::cout << crewName << ", Baker " << id << " is done with the ingredients.\n";

    // Wait for crew members to finish preparing ingredients
    barrier.arrive_and_wait();

    // Phase 2: Make Cake
    seconds = dists[crewId](gen);
    std::cout << crewName << ", Baker " << id << " is making the cake for " << seconds << " seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(seconds)); // Simulate task time

    std::cout << crewName << ", Baker " << id << " is done making the cake.\n";

    // Wait for crew members to finish making the cake
    barrier.arrive_and_wait();
}

int main() {
    // Create a barrier for each crew, since each crew can work independently on their cake
    std::barrier barrierCrewA(crewSize);
    std::barrier barrierCrewB(crewSize);

    // Fixed-size arrays for threads
    std::array<std::thread, crewSize> crewAThreads;
    std::array<std::thread, crewSize> crewBThreads;

    // Start threads for Crew A
    for (int i = 0; i < crewSize; ++i) {
        crewAThreads[i] = std::thread(bakingTask, 0, i + 1, std::ref(barrierCrewA));
    }

    // Start threads for Crew B
    for (int i = 0; i < crewSize; ++i) {
        crewBThreads[i] = std::thread(bakingTask, 1, i + 1, std::ref(barrierCrewB));
    }

    // Join threads for Crew A
    for (auto& thread : crewAThreads) {
        thread.join();
    }

    // Join threads for Crew B
    for (auto& thread : crewBThreads) {
        thread.join();
    }

    std::cout << "All cakes are done." << std::endl;

    return 0;
}
