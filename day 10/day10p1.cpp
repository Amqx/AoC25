//
// Created by Jonathan on 03-Dec-25.
//

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Machine {
    int required = 0;
    vector<int> buttons;
};

int ProcessRequired(const string &line) {
    int r = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '#') {
            r |= 1 << i;
        }
    }
    return r;
}

int CreateButton(const string &line) {
    int r = 0;

    for (const char &c: line) {
        if (c >= '0' && c <= '9') {
            r |= 1 << (c - '0');
        }
    }
    return r;
}

vector<int> ProcessButtons(string line) {
    vector<int> r;
    auto sep = line.find(')');
    while (sep != string::npos) {
        r.push_back(CreateButton(line.substr(1, sep - 1)));
        if (sep + 2 > line.length()) break;
        line = line.substr(sep + 2);
        sep = line.find(')');
    }
    return r;
}

Machine CreateMachine(const string &line) {
    const auto req_end = line.find(']');
    Machine r;
    r.required = ProcessRequired(line.substr(1, req_end - 1));
    const auto buttons_end = line.rfind('{');
    r.buttons = ProcessButtons(line.substr(req_end + 2, buttons_end - 3 - req_end));
    return r;
}

bool Test(const vector<int> &subset, const int &target) {
    int r = 0;
    for (const auto &b: subset) {
        r = r ^ b;
    }
    if (r == target) return true;
    return false;
}

int SolveMachine(const Machine &m) {
    const auto n = m.buttons.size();
    const auto v = m.buttons;
    const auto target = m.required;

    for (int k = 1; k <= n; k++) {
        vector sel(k, 1);
        sel.resize(n, 0);
        do {
            vector<int> subset;
            for (int i = 0; i < n; i++) {
                if (sel[i]) subset.push_back(v[i]);
            }

            if (Test(subset, target)) {
                return k;
            }
        } while (ranges::prev_permutation(sel).found);
    }

    return -1;
}

void PrintMachine(const Machine &m) {
    cout << "Solving machine with required " << m.required << endl;
    cout << "\tAvailable buttons: ";
    for (const auto &b: m.buttons) {
        cout << b << ' ';
    }
    cout << endl;
}

int main() {
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 10\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 10\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int iteration = 0; iteration < 50; iteration++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {
            string line;
            vector<Machine> machines;
            while (getline(input, line)) {
                machines.push_back(CreateMachine(line));
            }

            int sum = 0;
            for (const auto &m: machines) {
                const auto &t = SolveMachine(m);
                sum += t;
            }
            const auto t2 = chrono::steady_clock::now();

            cout << "Sum: " << sum << endl;
            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 5345.73us
    return 0;
}
