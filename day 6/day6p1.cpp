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

void AddRow(string line, vector<vector<uint64_t>>& v) {
    while (line[0] == ' ') {
        line = line.substr(1);
    }

    size_t sep = line.find(' ');
    vector<uint64_t> n;

    while (sep != string::npos) {
        const uint64_t i = stoull(line.substr(0, sep));
        n.push_back(i);
        line = line.substr(sep+1);
        while (line[0] == ' ') {
            line = line.substr(1);
        }
        sep = line.find(' ');
    }

    n.push_back(stoull(line));

    v.push_back(n);
}

uint64_t Perform(const vector<vector<uint64_t>>& r, const string& operators) {

    uint64_t sum = 0;

    for (int i = 0; i < operators.length(); i++) {
        const char operation = operators[i];
        vector<uint64_t> c;
        for (const auto& row : r) {
            c.push_back(row[i]);
        }
        switch (operation) {
            case '+': {
                for (const auto& v : c) {
                    sum += v;
                }
                break;
            }
            case '*': {
                uint64_t val = 1;
                for (const auto& v : c) {
                    val *= v;
                }
                sum += val;
                break;
            }
            default: break;
        }
    }

    return sum;
}

int main() {

    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 6\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 6\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }

    const auto t1 = chrono::high_resolution_clock::now();
    if (ifstream input(file); input.is_open()) {
        string line;
        vector<vector<uint64_t>> rows;
        while (getline(input, line) && line.find("+") == string::npos) {
            // add everything to rows first
            AddRow(line, rows);
        }

        const auto end = ranges::remove(line, ' ').begin();
        line.erase(end, line.end());

        const uint64_t sum = Perform(rows, line);
        input.close();

        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Answer: " << sum << endl; // ~ 0.0033s
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl;
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;


}