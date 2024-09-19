#include <iostream>
#include <thread>
#include <mutex>

std::mutex accountMutex;
int balance = 500;

void withdraw(int amount, const std::string& user) {
    std::unique_lock<std::mutex> lock(accountMutex, std::defer_lock);

    // Check balance before locking
    if (balance >= amount) {
        // Lock now, after condition is checked
        lock.lock();
        balance -= amount;
        std::cout << user << " successfully withdrew $" << amount << ". Remaining balance: $" << balance << std::endl;
        lock.unlock(); // Can unlock manually
    } else {
        std::cout << user << " attempted to withdraw $" << amount << ", but insufficient funds." << std::endl;
    }
}

int main() {
    std::thread userA(withdraw, 100, "User A");
    std::thread userB(withdraw, 100, "User B");

    userA.join();
    userB.join();
    
    return 0;
}
