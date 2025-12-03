//
// Created by Jonathan on 02-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

void pt2(int &count, int &curr, const string &line) {
    const string direction = line.substr(0, 1);
    const int turns = stoi(line.substr(1));
    const int step = direction == "R" ? 1 : -1;
    for (int i = 0; i < turns; i++) {
        curr += step;
        if (curr == 100) curr = 0;
        if (curr == -1) curr = 99;

        if (curr == 0) {
            count++;
        }
    }

}

int main() {
    const auto t1 = chrono::high_resolution_clock::now();
    if (ifstream input(R"(C:\Users\Jonathan\git\aoc25\day 1\input.txt)"); input.is_open()) {
        int curr = 50;
        int password = 0;
        string line;
        while (getline(input, line)) {
            pt2(password, curr, line);
        }
        input.close();
        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Number of 0s: " << to_string(password) << endl;
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl;
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;
}
