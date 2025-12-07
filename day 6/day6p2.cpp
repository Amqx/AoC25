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

void StripSpacesAtStart(string& input) {
    while (input[0] == ' ') input = input.substr(1);
}

void StripAllSpaces(string& input) {
    const auto end = ranges::remove(input, ' ').begin();
    input.erase(end, input.end());
}

vector<vector<string>> NormalizeRows(vector<string> in) {
    vector<vector<string>> out;

    int longest = 0;
    for (const auto& s : in) {
        if (const auto len = s.length(); len > longest) longest = len;
    }
    for (auto& s : in) {
        while (s.length() < longest) s += ' ';
    }

    vector<string> col;
    const size_t len = in.capacity();
    col.resize(len);

    for (int i = 0; i < longest; i++) {
        bool end = true;

        for (const auto& r : in) {
            if (r[i] != ' ') {
                for (int j = 0; j < len; j++) {
                    col[j] += in[j][i];
                }
                end = false;
                break;
            }
        }

        if (end) {
            out.push_back(col);
            col.assign(len, string{});
        }
    }
    out.push_back(col);
    return out;
}

vector<vector<uint64_t>> Invert(const vector<vector<string>> &cols) {
    vector<vector<uint64_t>> out;

    for (const auto& c : cols) {
        size_t len = 0;
        for (const auto& s : c) {
            if (const auto temp = s.length(); temp > len) len = temp;
        }

        vector<uint64_t> ints;
        ints.reserve(len);

        for (int i = 0; i < len; i++) {
            string temp;
            for (const auto& s : c) {
                temp += s[i];
            }
            StripAllSpaces(temp);
            ints.push_back(stoull(temp));
        }
        out.push_back(ints);
    }

    return out;
}

uint64_t Perform(const vector<vector<uint64_t>>& c, const string& operators) {
    uint64_t sum = 0;
    for (int i = 0; i < operators.length(); i++) {
        switch (operators[i]) {
            case '+': {
                for (const auto& v : c[i]) {
                    sum += v;
                }
                break;
            }
            case '*': {
                uint64_t val = 1;
                for (const auto& v : c[i]) {
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
    double total = 0;
    for (int i = 0; i < 50; i++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {
            string line;
            vector<string> in;
            while (getline(input, line) && line.find("+") == string::npos) {
                in.push_back(line);
            }

            const auto inverted = Invert(NormalizeRows(in));
            StripAllSpaces(line);

            const auto sum = Perform(inverted, line);
            input.close();

            const auto t2 = chrono::steady_clock::now();
            cout << "HW Answer: " << sum << endl;
            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }

    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 1032.29us
    return 0;
}