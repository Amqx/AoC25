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
            int sum = 0;
            string line;
            while (getline(input, line)) {
                const int val = part1(line);
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
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 410.22us
    return 0;
}
