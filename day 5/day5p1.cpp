//
// Created by Jonathan on 03-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <ranges>
#include <algorithm>

using namespace std;

void GetRanges(const string& line, vector<vector<uint64_t>>& ranges) {
    const size_t sep = line.find('-');
    const uint64_t val1 = stoull(line.substr(0, sep));
    const uint64_t val2 = stoull(line.substr(sep+1));
    ranges.push_back(vector{val1, val2});
}

bool InRange(const vector<vector<uint64_t>>& ranges, const uint64_t val) {
    return ranges::any_of(ranges, [val](const auto& r) {
        return val >= r[0] && val <= r[1];
    });
}

int main() {

    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 5\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 5\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }

    const auto t1 = chrono::high_resolution_clock::now();
    if (ifstream input(file); input.is_open()) {
        uint64_t sum = 0;
        string line;
        vector<vector<uint64_t>> ranges;
        while (getline(input, line) && !line.empty()) {
            GetRanges(line, ranges);
        }

        while (getline(input, line)) {
            if (InRange(ranges, stoull(line))) {
                sum++;
            }
        }

        input.close();
        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Fresh ingredients: " << sum << endl; // ~0.0016s
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl;
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;


}