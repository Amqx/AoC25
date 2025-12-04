//
// Created by Jonathan on 02-Dec-25.
//

#include <string>
#include <vector>
#include <iostream>
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

uint64_t LowHelper(const uint64_t n, const uint64_t d) {
    const uint64_t left = n / ipow10(d/2);
    const uint64_t right = n % ipow10(d/2);

    if (left >= right) return left;

    return left + 1;
}

uint64_t HighHelper(const uint64_t n, const uint64_t d) {
    const uint64_t left = n / ipow10(d/2);
    const uint64_t right = n % ipow10(d/2);

    if (left <= right) return left;

    return left-1;
}

uint64_t repeated(uint64_t low, uint64_t high) {
    uint64_t l_d = digits(low);
    uint64_t h_d = digits(high);
    
    if (l_d % 2 != 0 && h_d % 2 != 0 && l_d == h_d) return 0;
    
    if (l_d % 2 != 0) {
        low = ipow10(l_d);
        l_d++;
    }

    if (h_d % 2 != 0) {
        high = ipow10(h_d-1) - 1;
        h_d--;
    }

    uint64_t sum = 0;

    if (l_d == h_d) {
        const uint64_t h = l_d / 2;
        const uint64_t step = ipow10(h);
        const uint64_t sublow = LowHelper(low, l_d);
        const uint64_t subhigh = HighHelper(high, h_d);

        for (uint64_t i = sublow; i <= subhigh; i++) {
            sum += i + step * i;
        }
        return sum;
    }

    vector<uint64_t> subregions;
    for (uint64_t i = l_d; i <= h_d; i+=2) {
        subregions.push_back(i);
    }

    for (const uint64_t& i : subregions) {
        if (i == l_d) {
            const uint64_t step = ipow10(i/2);

            const uint64_t sublow = LowHelper(low, l_d);
            const uint64_t subhigh = step - 1;

            for (uint64_t j = sublow; j <= subhigh; j++) {
                sum += j + step * j;
            }

            continue;
        }

        if (i == h_d) {
            const uint64_t step = ipow10(i/2);

            const uint64_t sublow = step / 10;
            const uint64_t subhigh = HighHelper(high, h_d);

            for (uint64_t j = sublow; j <= subhigh; j++) {
                sum += j + step * j;
            }

            continue;
        }

        const uint64_t step = ipow10(i/2);

        const uint64_t sublow = step / 10;
        const uint64_t subhigh = step - 1;

        for (uint64_t j = sublow; j <= subhigh; j++) {
            sum += j + step * j;
        }
    }

    return sum;

}

int main() {
    const auto t1 = chrono::high_resolution_clock::now();
    string line;
    if (ifstream input(R"(C:\Users\Jonathan\git\aoc25\day 2\input.txt)"); input.is_open()) {
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
    const auto t2 = chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
    cout << "Sum: " << sum << endl;
    cout << "Time (s): " << duration / 1000 << endl; // ~0.0033s
    return 0;
}