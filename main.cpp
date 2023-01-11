#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>

using namespace std;

mutex mtx;

vector<int> findPrimeNumbersInterval(int start, int end) {
    //check if number is even
    if (start ^ (1 == start + 1)) {
        start = start + 1;
    }

    vector<int> primes;

    for (int target = start; target < end; target += 2) {
        bool isPrime = true;
        int target_squared = (int) sqrt(target);

        for (int i = 3; i < target_squared; i += 2) {
            if (target % i == 0) {
                isPrime = false;
                break;
            }
        }

        if (isPrime) {
            primes.emplace_back(target);
        }
    }
    return primes;
}

void printResults(double duration, const vector<int>& primes) {
    cout << "Program finish.\n";
    cout << "Time taken:      " << duration << " s\n";
    cout << "Found :          " << primes.size() << " primes \n";

}

int main() {
    int start = 0;
    int end = 50000000;
    int intervalLength = 20000;


    vector<thread> threads;
    vector<int> primes;


    auto start_time = chrono::steady_clock::now();
    int i = 0;

    cout << "Finding primes between " << start << " and " << end << "...\n";

    while (i < end) {
        threads.emplace_back(
                [i, &intervalLength, &primes] {
                    vector<int> found_primes = findPrimeNumbersInterval(i, i + intervalLength);
                    std::lock_guard<std::mutex> lock(mtx);
                    primes.insert(primes.end(), found_primes.begin(), found_primes.end());
                });

        i += intervalLength;
    }

    for (auto &thread: threads) {
        thread.join();
    }

    auto end_time = chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    printResults(duration.count(), primes);
    return 0;
}




