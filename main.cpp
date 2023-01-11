#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>

using namespace std;

mutex mtx;

bool isPrime(int num) {
    if (num <= 2) {
        return false;
    }
    if ((num & 1) == 0) {
       return false;
    }

    int num_sqrt = (int) sqrt(num);

    for (int i = 3; i <= num_sqrt; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

void printResults(double duration, const vector<int> &primes) {
    cout << "Program finish.\n";
    cout << "Time taken:      " << duration << " s\n";
    cout << "Found :          " << primes.size() << " primes \n";

}

int main() {
    int start = 0;
    int end = 10000000;
    int intervalLength = 20000;


    vector<thread> threads;
    vector<int> primes;
    for (int i = 1; i < 100; i++) {
        if (isPrime(i)) {
            cout << i << "\n";
        }
    }

    auto start_time = chrono::steady_clock::now();
    int i = 0;


    cout << "Finding primes between " << start << " and " << end << "...\n";

    while (i < end) {
        threads.emplace_back(
                [i, &intervalLength, &primes] {
                    int start = i;
                    if (i ^ (1 == i + 1)) {
                        start++;
                    }
                    for (int j = start; j < start + intervalLength; j += 2) {
                        if (isPrime(j)) {
                            std::lock_guard<std::mutex> lock(mtx);
                            primes.push_back(j);
                        }
                    }
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




