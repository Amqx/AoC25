//
// Created by Jonathan on 07-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

int main() {

    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 9\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 9\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int iteration = 0; iteration < 50; iteration++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {

            string line;
            vector<pair<int, int>> points;
            while (getline(input, line)) {
                const auto sep = line.find(',');
                const auto x = stoi(line.substr(0, sep));
                const auto y = stoi(line.substr(sep+1));
                const pair<int, int> n(x,y);
                points.push_back(n);
            }

            uint64_t area = 0;
            for (int i = 0; i < points.size(); i++) {
                const auto [x1, y1] = points[i];

                for (int j = i + 1; j < points.size(); j++) {
                    const auto [x2, y2] = points[j];
                    const uint64_t A = static_cast<uint64_t>(abs(x2 - x1) + 1) * static_cast<uint64_t>(abs(y2 - y1) + 1);
                    if (A > area) area = A;
                }
            }

            const auto t2 = chrono::steady_clock::now();
            cout << "Area: " << area << endl;

            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 536.06us
    return 0;
}