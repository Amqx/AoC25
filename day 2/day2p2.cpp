//
// Created by Jonathan on 02-Dec-25.
//

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <fstream>
#include <chrono>

using namespace std;

uint64_t ipow10(unsigned n) {
    uint64_t r = 1;
    while (n--) r *= 10;
    return r;
}

uint64_t digits(uint64_t n) {
    unsigned d = 1;
    while (n >= 10) {
        n /= 10;
        d++;
    }
    return d;
}

vector<int> factors(const uint64_t n) {
    vector<int> r;

    for (uint64_t i = 1; i <= n/2; i++) {
        if (n % i == 0) {
            r.push_back(i);
        }
    }

    return r;
}

vector<vector<uint64_t>> GetRegions(string regions) {
    size_t sep = regions.find(",");
    vector<vector<uint64_t>> returned;
    while (sep != string::npos) {
        string reg = regions.substr(0, sep);
        regions = regions.substr(sep+1);
        sep = regions.find(",");
        const size_t regsep = reg.find("-");
        const uint64_t num1 = stoull(reg.substr(0, regsep));
        const uint64_t num2 = stoull(reg.substr(regsep+1));
        returned.push_back(vector{num1, num2});
    }
    const size_t regsep = regions.find("-");
    const uint64_t num1 = stoull(regions.substr(0, regsep));
    const uint64_t num2 = stoull(regions.substr(regsep+1));
    returned.push_back(vector{num1, num2});
    return returned;
}

uint64_t repeated(uint64_t low, uint64_t high) {
    uint64_t l_d = digits(low);
    uint64_t h_d = digits(high);
    uint64_t sum = 0;

    for (size_t d = l_d; d <= h_d; d++) {
        uint64_t rlow = (d == l_d) ? low : ipow10(d-1);
        uint64_t rhigh = (d == h_d) ? high : ipow10(d) -1;

        if (low > high) continue;

        set<uint64_t> seen;

        for (size_t len = 1; len * 2 <= d; len++) {
            if (d % len != 0) continue;
            const uint64_t k = d / len;

            uint64_t multi = 0;
            const uint64_t step = ipow10(len);
            for (size_t i = 0; i < k; i++) {
                multi = multi * step + 1;
            }

            const uint64_t internal_low = ipow10(len - 1);
            const uint64_t internal_high = ipow10(len) - 1;

            uint64_t internal_min = (rlow + multi - 1) / multi;
            uint64_t internal_max = rhigh / multi;

            if (internal_min < internal_low) internal_min = internal_low;
            if (internal_max > internal_high) internal_max = internal_high;
            if (internal_min > internal_max) continue;

            for (uint64_t s = internal_min; s <= internal_max; s++) {
                const uint64_t newnum = s * multi;

                if (seen.insert(newnum).second) {
                    sum += newnum;
                }
            }
        }

    }

    return sum;
}

int main() {
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 2\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 2\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int i = 0; i < 50; i++) {
        const auto t1 = chrono::steady_clock::now();
        string line;
        if (ifstream input(file); input.is_open()) {
            getline(input, line);
            input.close();
        } else {
            cout << "Failed to open file" << endl;
            return 1;
        }
        const auto regions = GetRegions(line);

        uint64_t sum = 0;

        for (const auto& curr : regions) {
            const uint64_t low = curr[0];
            const uint64_t high = curr[1];
            sum += repeated(low, high);
        }
        const auto t2 = chrono::steady_clock::now();
        cout << "Sum: " << sum << endl;
        total += std::chrono::duration<double, micro>(t2 - t1).count();
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 384.55us
    return 0;
}