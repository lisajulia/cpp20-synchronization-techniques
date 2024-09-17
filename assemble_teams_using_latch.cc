#include <algorithm>
#include <chrono>
#include <iostream>
#include <latch>
#include <memory>
#include <random>
#include <thread>
#include <vector>

std::random_device rd;   // Non-deterministic random number generator
std::mt19937 gen(rd());  // Seed the generator
std::uniform_int_distribution<> dist(1, 5);  // Distribution for random wait time

class TeamManager {
public:
    TeamManager(int team_size, int number_of_teams)
        : team_size_(team_size)
        , number_of_teams_(number_of_teams)
    {
        latches_.reserve(number_of_teams_);
        for (int i = 0; i < number_of_teams_; ++i) {
            auto latch = std::make_shared<std::latch>(team_size_);
            latches_.push_back(latch);
        }
    }

    void member_arrives(int member_id, int team_id) {
        // Assign member to the chosen team, starting from team 1 to 4
        std::cout << "Member " << member_id << " is assigned to Team " << team_id + 1 << "." << std::endl;
        latches_[team_id]->count_down();
    }

    void wait_for_teams() {
        std::for_each(latches_.begin(), latches_.end(), [](const auto& latch) { latch->wait(); });
    }

private:
    int team_size_;
    int number_of_teams_;
    std::vector<std::shared_ptr<std::latch>> latches_;
};

void member_thread(TeamManager& manager, int member_id, int team_id) {
    auto waitingTime = dist(gen);
    std::cout << "Member " << member_id << " is on the way... that will take " << waitingTime << " s." << std::endl;
    // Simulate random travel time
    std::this_thread::sleep_for(std::chrono::seconds(waitingTime));
    
    manager.member_arrives(member_id, team_id);
}

int main() {
    const int team_size = 3;        // Number of members required per team
    const int number_of_teams = 4;  // Number of teams

    TeamManager manager(team_size, number_of_teams);

    std::cout << "Calling the team members..." << std::endl;

    // Create threads for members arriving
    std::vector<std::thread> members;
    for (int i = 0; i < team_size * number_of_teams; ++i) {
        members.emplace_back(member_thread, std::ref(manager), i + 1, i % number_of_teams);
    }

    std::cout << "Waiting for all team members to arrive and get assigned..." << std::endl;
    // Manager starts to wait, since now the "members are in the pipeline"
    manager.wait_for_teams();

    // Join all member threads, i.e. start the actual arrival process, concurrently for all members
    for (auto& member : members) {
        member.join();
    }
    
    std::cout << "All teams are ready to play! The game begins!" << std::endl;

    return 0;
}
