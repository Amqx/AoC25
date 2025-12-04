//
// Created by Jonathan on 03-Dec-25.
//

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

string largest(const string& line, const int& n) {
    for (int i = 9; i >= 0; i--) {
        string num = to_string(i);
        const size_t pos = line.find(num);
        if (pos != string::npos) {
            if (pos + n + 1 > line.length()) continue;
            return num;
        }
    }
    return "end";
}

uint64_t part2(const string& line) {

    const string largest_digit = largest(line, 11);

    string returned = largest_digit;
    string substr = line.substr(line.find(largest_digit) + 1);

    while (returned.length() < 12) {
        const string sublargest = largest(substr, 11 - returned.length());
        returned += sublargest;

        substr = substr.substr(substr.find(sublargest) + 1);
    }

    return stoull(returned);

}


int main() {
    const auto t1 = chrono::high_resolution_clock::now();
    if (ifstream input(R"(C:\Users\Jonathan\git\aoc25\day 3\input.txt)"); input.is_open()) {
        uint64_t sum = 0;
        string line;
        while (getline(input, line)) {
            const uint64_t val = part2(line);
            sum += val;
        }
        input.close();
        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Joltage: " << sum << endl;
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl; // ~0.0016s
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;
}