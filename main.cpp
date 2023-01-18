#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>

using namespace std;

mutex mtx;

bool isPrime(int n) {
    if (n <= 1) return false;
    //if (n <= 3) return true;
    //if (n == 5) return true;
    //if (n == 7) return true;
    if (n % 3 == 0 || n % 5 == 0 || n % 7 == 0) return false;

    int sqrt_n = (int) sqrt(n);
    for (int i = 5; i <= sqrt_n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void findPrimesInInterval(int start, int end, vector<int> &primes) {
    int pointer = start;

    //check if number is even except number 2
    if ((start & 1) == 0) {
        pointer++;

        for (int j = pointer; j < end; j += 2) {
            if (isPrime(j)) {
                std::lock_guard<std::mutex> lock(mtx);
                primes.emplace_back(j);
            }
        }
    }
}

void printResults(double duration, const vector<int> &primes) {
    cout << "Program finish.\n";
    cout << "Time taken:      " << duration << " s\n";
    cout << "Found :          " << primes.size() << " primes \n";

}

void test() {
    for (int i = 1; i < 10; i++) {
        if (isPrime(i)) {
            cout << i << "\n";
        }
    }
}

int main() {
    int start = 2;
    int end = 1000000;
    int intervalLength = 50000;

    //test();

    int i = 0;
    vector<thread> threads;

    //These primes are not checked for in is prime
    vector<int> primes = {2, 3, 5, 7};

    cout << "Finding primes between " << start << " and " << end << "...\n";

    auto start_time = chrono::steady_clock::now();

    //Create threads with an array of numbers with length intervalLength
    while (i < end) {
        threads.emplace_back([i, intervalLength, &primes] { findPrimesInInterval(i, i + intervalLength, primes); });
        i += intervalLength;
    }

    //Do the rest of the numbers if there are any
    if (i != end) {
        threads.emplace_back([i, end, &primes] { findPrimesInInterval(i, end, primes); });
    }

    //Wait for all threads to finish
    for (auto &thread: threads) {
        thread.join();
    }

    auto end_time = chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    printResults(duration.count(), primes);

    return 0;
}




