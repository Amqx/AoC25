//
// Created by Jonathan on 03-Dec-25.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

void GetRanges(const string& line, vector<vector<uint64_t>>& ranges) {
    const size_t sep = line.find('-');
    const uint64_t val1 = stoull(line.substr(0, sep));
    const uint64_t val2 = stoull(line.substr(sep+1));
    ranges.push_back(vector{val1, val2});
}

uint64_t Fresh(vector<vector<uint64_t>>& ranges) {
    // need to remove all dupes
    ranges::sort(ranges);

    vector<vector<uint64_t>> merged;
    merged.push_back(ranges[0]);

    for (size_t i = 1; i < ranges.size(); i++) {
        auto& last = merged.back();
        if (const auto& curr = ranges[i]; curr[0] <= last[1] + 1) {
            last[1] = max(last[1], curr[1]);
        } else {
            merged.push_back(curr);
        }
    }

    uint64_t fresh = 0;
    for (const auto& r : merged) {
        fresh += 1 + r[1] - r[0];
    }
    return fresh;
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

        sum = Fresh(ranges);

        input.close();
        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Fresh ingredients: " << sum << endl; // ~0.00108s
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl;
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;
}