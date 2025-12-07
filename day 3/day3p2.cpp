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
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 3\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 3\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int i = 0; i < 50; i++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {
            uint64_t sum = 0;
            string line;
            while (getline(input, line)) {
                const uint64_t val = part2(line);
                sum += val;
            }
            input.close();
            const auto t2 = chrono::steady_clock::now();
            cout << "Joltage: " << sum << endl;
            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 897.93us
    return 0;
}