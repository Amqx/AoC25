//
// Created by Jonathan on 03-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

struct splitter {
    int row;
    int col;

    bool operator==(const splitter &other) const {
        if (row == other.row && col == other.col) return true;
        return false;
    }
};

int recurse(const vector<string>& rows, vector<splitter>& hit, const int& line_num, const int& beam, const size_t& len) {
    // base case: finished the last row
    // base case: beam is out of bounds
    if (line_num >= rows.size()) return 0;
    if (beam < 0 || beam >= len) return 0;

    int curr = line_num;
    while (curr < rows.size()) {
        if (rows[curr][beam] == '^') {
            const auto loc = splitter{curr, beam};
            int splits = 0;
            if (ranges::find(hit, loc) != hit.end()) {
                return splits;
            }
            splits++;
            hit.push_back(loc);
            splits += recurse(rows, hit, curr+1, beam - 1, len);
            splits += recurse(rows, hit, curr+1, beam + 1, len);

            return splits;
        }
        curr++;
    }
    return 0;
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
            int sum = 0;
            string line;
            vector<string> rows;
            while (getline(input, line)) {
                rows.push_back(line);
            }
            input.close();

            // for the first line, simply create a beam below the S
            const size_t initial = rows[0].find('S');
            vector<splitter> hit;
            sum += recurse(rows, hit, 2, initial, rows[0].length());

            const auto t2 = chrono::steady_clock::now();
            cout << "Splits: " << sum << endl;
            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 2213.41us
    return 0;
}