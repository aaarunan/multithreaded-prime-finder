#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>

using namespace std;

mutex mtx;

bool isPrime(int num) {
    //Not needed in a controlled environment
    //if (num == 2) {
    //    return true;
    //}
    //if ((num & 1) == 0) {
    //    return false;
    //}
    //if (num <= 1) {
    // return false;
    //}

    int num_sqrt = (int) sqrt(num);

    for (int i = 3; i <= num_sqrt; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }

    return true;
}

void findPrimes(int num, int intervalLength, vector<int> &primes) {
    int start = num;

    //check if number is even except number 2
    if ((num & 1) == 0) {
        start++;
    }


    for (int j = start; j < start + intervalLength; j += 2) {
        if (isPrime(j)) {
            std::lock_guard<std::mutex> lock(mtx);
            primes.emplace_back(j);
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
    int end = 100000000;
    int intervalLength = 100000;

    //test();

    int i = 0;
    vector<thread> threads;
    vector<int> primes;

    cout << "Finding primes between " << start << " and " << end << "...\n";

    auto start_time = chrono::steady_clock::now();

    if (start <= 2) {
        primes.push_back(2);
    }

    //Create threads with an array of numbers with length intervalLength
    while (i < end) {
        threads.emplace_back([i, intervalLength, &primes] { findPrimes(i, intervalLength, primes); });
        i += intervalLength;
    }

    //Do the rest of the numbers if there are any
    if (i != end) {
        int rest = end - i;
        threads.emplace_back([i, rest, &primes] { findPrimes(i, rest, primes); });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    auto end_time = chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    printResults(duration.count(), primes);

    return 0;
}




