//
// Created by Jonathan on 03-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

struct path {
    uint64_t row;
    uint64_t column;
    uint64_t sum;

    bool operator==(const path &o) const {
        return (row == o.row && column == o.column);
    }
};

uint64_t recurse(const vector<string>& rows, path curr, const size_t& len, vector<path>& cache) {
    // base case: finished the last row
    // base case: beam is out of bounds
    if (curr.row >= rows.size()) return 1;
    if (curr.column < 0 || curr.column >= len) return 1;

    // if already cached just take that result
    if (const auto pos = ranges::find(cache, curr); pos != cache.end()) {
        return pos->sum;
    }

    uint64_t row = curr.row;
    const uint64_t col = curr.column;
    while (row < rows.size()) {
        if (rows[row][col] == '^') {
            const auto nright = path{row+1, col+1};
            const auto nleft = path{row+1, col-1};
            const uint64_t total = recurse(rows, nleft, len, cache) + recurse(rows, nright, len, cache);
            curr.sum = total;
            cache.push_back(curr);
            return total;
        }
        row++;
    }
    return 1;
}

int main() {

    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 7\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 7\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int i = 0; i < 50; i++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {
            string line;
            vector<string> rows;
            while (getline(input, line)) {
                rows.push_back(line);
            }
            input.close();

            // for the first line, simply create a beam below the S
            const auto init = path{2, rows[0].find('S'), 0};
            vector<path> c;
            const uint64_t sum = recurse(rows, init, rows[0].length(), c);

            const auto t2 = chrono::steady_clock::now();
            cout << "Timelines: " << sum << endl;
            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 3640.52us
    return 0;
}