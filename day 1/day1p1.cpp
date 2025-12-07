//
// Created by Jonathan on 02-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

void pt1(int &count, int &curr, const string &line) {
    const string direction = line.substr(0, 1);
    const int turns = stoi(line.substr(1));
    const int step = direction == "R" ? 1 : -1;
    for (int i = 0; i < turns; i++) {
        curr += step;
        if (curr == 100) curr = 0;
        if (curr == -1) curr = 99;
    }
    if (curr == 0) {
        count++;
    }
}

int main() {
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 1\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 1\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int i = 0; i < 50; i++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {
            int curr = 50;
            int password = 0;
            string line;
            while (getline(input, line)) {
                pt1(password, curr, line);
            }
            input.close();
            const auto t2 = chrono::steady_clock::now();
            cout << "Number of 0s: " << to_string(password) << endl;
            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 2025.54us
    return 0;
}
