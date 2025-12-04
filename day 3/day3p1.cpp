//
// Created by Jonathan on 03-Dec-25.
//

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

string largest(const string& line) {
    for (int i = 9; i >= 0; i--) {
        string num = to_string(i);
        const size_t pos = line.find(num);
        if (pos != string::npos) {
            if (pos == line.length() - 1) continue;
            return num;
        }
    }
    return "0";
}

string sublargest(const string& line) {
    for (int i = 9; i >= 0; i--) {
        string num = to_string(i);
        const size_t pos = line.find(num);
        if (pos != string::npos) return num;
    }
    return "0";
}

int part1(const string& line) {
    const string greatest = largest(line);

    size_t sep = line.find(greatest);
    const string substr = line.substr(sep+1);
    const string second_greatest = sublargest(substr);

    return stoi(greatest + second_greatest);
}

int main() {
    const auto t1 = chrono::high_resolution_clock::now();
    if (ifstream input(R"(C:\Users\Jonathan\git\aoc25\day 3\input.txt)"); input.is_open()) {
        int sum = 0;
        string line;
        while (getline(input, line)) {
            const int val = part1(line);
            sum += val;
        }
        input.close();
        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Joltage: " << sum << endl;
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl; // ~0.00054s
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;
}
