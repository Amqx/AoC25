//
// Created by Jonathan on 03-Dec-25.
//

//
// Created by Jonathan on 07-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <ranges>

using namespace std;

struct device {
    string id;
    vector<string> attached;
    bool output = false;
    int paths = -1;
};

void CreateDevice(const string &line, device &starts, vector<device> &devices) {
    auto sep = line.find(':');
    const auto id = line.substr(0, sep);
    auto connections = line.substr(sep + 2);
    vector<string> connects;
    device r;
    r.id = id;
    if (connections == "out") {
        r.output = true;
        devices.emplace_back(r);
        return;
    }
    sep = connections.find(' ');
    while (sep != string::npos) {
        const auto new_connection = connections.substr(0, sep);
        connects.emplace_back(new_connection);
        connections = connections.substr(sep + 1);
        sep = connections.find(' ');
    }
    connects.emplace_back(connections);
    r.attached = connects;

    if (id == "you") {
        starts = r;
        return;
    }

    devices.emplace_back(r);
}

void PrintDevice(const device &d) {
    cout << "Device " << d.id << "(" << d.output << ")" << endl;
    cout << "Attached to: ";
    for (const auto &s: d.attached) {
        cout << s << ' ';
    }
    cout << endl;
}

int Recurse(device &d, vector<device> &devices) {
    // if the current device is an output return 1
    if (d.output) {
        d.paths = 1;
        return 1;
    }
    // if the current device is already solved return its total
    if (d.paths != -1) {
        return d.paths;
    }

    int total = 0;
    for (device &temp: devices) {
        for (auto &next: d.attached) {
            if (temp.id == next) {
                total += Recurse(temp, devices);
                break;
            }
        }
    }
    d.paths = total;
    return total;
}

int main() {
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 11\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 11\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int iteration = 0; iteration < 50; iteration++) {
        const auto t1 = chrono::steady_clock::now();
        ifstream input(file);
        if (!input.is_open()) {
            cout << "Failed to open file!" << endl;
            return 1;
        }
        string line;
        device start;
        vector<device> devices;
        while (getline(input, line)) {
            CreateDevice(line, start, devices);
        }

        const auto r = Recurse(start, devices);

        cout << "Total paths: " << r << endl;
        const auto t2 = chrono::steady_clock::now();

        total += std::chrono::duration<double, micro>(t2 - t1).count();
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 1113.36us

    return 0;
}
