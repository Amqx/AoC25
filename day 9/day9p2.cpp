//
// Created by Jonathan on 07-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

bool Inside(const double &px, const double &py, const vector<pair<uint64_t, uint64_t> > &poly, const size_t &n) {
    int crossings = 0;

    for (size_t i = 0; i < n; i++) {
        auto p1 = poly[i];
        auto p2 = poly[(i + 1) % n];

        if (p1.second > p2.second) swap(p1, p2);

        if (py >= p1.second && py < p2.second) {
            if (p1.first == p2.first) {
                if (px < p1.first) {
                    crossings++;
                }
            }
        }
    }
    return crossings % 2 == 1;
}

bool Valid(const uint64_t &min_x, const uint64_t &max_x, const uint64_t &min_y, const uint64_t max_y,
           const vector<pair<uint64_t, uint64_t> > &poly, const size_t &n) {
    const double mid_x = (min_x + max_x) / 2.0;
    const double mid_y = (min_y + max_y) / 2.0;
    if (!Inside(mid_x, mid_y, poly, n)) return false;

    for (const auto &p: poly) {
        if (p.first > min_x && p.first < max_x && p.second > min_y && p.second < max_y) {
            return false;
        }
    }

    for (size_t i = 0; i < n; i++) {
        const auto p1 = poly[i];
        const auto p2 = poly[(i + 1) % n];

        if (p1.first == p2.first) {
            if (p1.first > min_x && p1.first < max_x) {
                const uint64_t edge_min_y = min(p1.second, p2.second);
                const uint64_t edge_max_y = max(p1.second, p2.second);
                if (max(min_y, edge_min_y) < min(max_y, edge_max_y)) {
                    return false;
                }
            }
        } else if (p1.second == p2.second) {
            if (p1.second > min_y && p1.second < max_y) {
                const uint64_t edge_min_x = min(p1.first, p2.first);
                const uint64_t edge_max_x = max(p1.first, p2.first);
                if (max(min_x, edge_min_x) < min(max_x, edge_max_x)) {
                    return false;
                }
            }
        }
    }

    return true;
}

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
            vector<pair<uint64_t, uint64_t> > reds;
            while (getline(input, line)) {
                const auto sep = line.find(',');
                const auto x = stoi(line.substr(0, sep));
                const auto y = stoi(line.substr(sep + 1));
                reds.emplace_back(x, y);
            }

            const auto n = reds.size();
            uint64_t area = 0;

            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    const auto p1 = reds[i];
                    const auto p2 = reds[j];
                    if (p1.first == p2.first || p1.second == p2.second) continue;

                    const auto x1 = p1.first;
                    const auto x2 = p2.first;
                    const auto y1 = p1.second;
                    const auto y2 = p2.second;

                    const auto min_x = min(x1, x2);
                    const auto max_x = max(x1, x2);
                    const auto min_y = min(y1, y2);
                    const auto max_y = max(y1, y2);
                    const auto width = max_x - min_x + 1;
                    const auto height = max_y - min_y + 1;
                    const auto a = width * height;

                    if (a <= area) continue;

                    if (Valid(min_x, max_x, min_y, max_y, reds, n)) {
                        area = a;
                    }
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
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 145382.64us
    return 0;
}
