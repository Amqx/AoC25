//
// Created by Jonathan on 03-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

bool GetAll(ifstream& in, string& prev, string& curr, string& next) {
    static bool final = false;
    if (final) return false;

    prev = curr;
    curr = next;
    if (getline(in, next)) return true;
    if (in.eof()) {
        final = true;
        next = "";
        return true;
    }

    return false;
}

int main() {

    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 4\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 4\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }

    const auto t1 = chrono::high_resolution_clock::now();
    if (ifstream input(file); input.is_open()) {
        int sum = 0;
        string prev, curr, next;

        getline(input, curr);
        getline(input, next);

        int length = curr.length();
        const auto empty = string(length, '.');

        do {
            if (prev.empty()) {
                prev = empty;
            }

            if (next.empty()) {
                next = empty;
            }

            for (int i = 0; i < length; i++) {
                if (curr[i] == '@') {
                    int rolls = 0;
                    if (i == 0) {
                        for (int k = i; k <= i+1; k++) {
                            if (curr[k] == '@') rolls++;
                            if (prev[k] == '@') rolls++;
                            if (next[k] == '@') rolls++;
                        }
                    } else if (i == length - 1) {
                        for (int k = i - 1; k <= i; k++) {
                            if (curr[k] == '@') rolls++;
                            if (prev[k] == '@') rolls++;
                            if (next[k] == '@') rolls++;
                        }
                    } else {
                        for (int k = i-1; k <= i+1; k++) {
                            if (curr[k] == '@') rolls++;
                            if (prev[k] == '@') rolls++;
                            if (next[k] == '@') rolls++;
                        }
                    }

                    if (rolls < 5) {
                        sum += 1;
                    }
                }
            }
        } while (GetAll(input, prev, curr, next));

        input.close();
        const auto t2 = chrono::high_resolution_clock::now();
        cout << "Rolls: " << sum << endl; // ~0.0010s
        const auto duration = std::chrono::duration<double, std::milli>(t2 - t1).count();
        cout << "Time (s): " << duration / 1000 << endl;
        return 0;
    }
    cout << "Failed to open input file" << endl;
    return 1;


}