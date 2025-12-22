//
// Created by Jonathan on 03-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <array>

using namespace std;

struct device {
    string id;
    vector<string> attached;
    bool output = false;
};

void CreateDevice(const string &line, vector<device> &devices) {
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
    ranges::sort(connects);
    connects.erase(ranges::unique(connects).begin(), connects.end());
    r.attached = connects;

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

uint64_t Recurse(const int n, bool seen_dac, bool seen_fft, const vector<device> &devices,
                 const vector<vector<int> > &adj,
                 vector<array<array<uint64_t, 2>, 2> > &cache, vector<int> visited) {
    if (visited[n]) return 0;
    if (devices[n].id == "fft") {
        seen_fft = true;
    }
    if (devices[n].id == "dac") {
        seen_dac = true;
    }

    auto &r = cache[n][seen_dac][seen_fft];
    if (r != UINT64_MAX) {
        return r;
    }

    visited[n] = 1;

    uint64_t total = 0;

    if (devices[n].output) {
        total = (seen_dac && seen_fft) ? 1 : 0;
    } else {
        for (const auto &i: adj[n]) total += Recurse(i, seen_dac, seen_fft, devices, adj, cache, visited);
    }

    visited[n] = 0;
    r = total;
    return r;
}

auto Solve(const vector<device> &devices) {
    const auto n = devices.size();
    unordered_map<string, int> idx;
    idx.reserve(n);
    for (int i = 0; i < n; i++) {
        idx[devices[i].id] = i;
    }

    vector<vector<int> > adj(n);
    for (int i = 0; i < n; i++) {
        for (const string &next: devices[i].attached) {
            auto it = idx.find(next);
            if (it != idx.end()) {
                adj[i].push_back(it->second);
            }
        }
        ranges::sort(adj[i]);
        adj[i].erase(ranges::unique(adj[i]).begin(), adj[i].end());
    }

    vector<array<array<uint64_t, 2>, 2> > cache(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                cache[i][j][k] = UINT64_MAX;
            }
        }
    }

    vector<int> visited(n, 0);
    return Recurse(idx.at("svr"), false, false, devices, adj, cache, visited);
}

int main() {
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 11\testp2.txt)";
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
        vector<device> devices;
        while (getline(input, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            CreateDevice(line, devices);
        }

        const auto &r = Solve(devices);

        cout << "Total paths: " << r << endl;
        const auto t2 = chrono::steady_clock::now();

        total += std::chrono::duration<double, micro>(t2 - t1).count();
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 1370.16us

    return 0;
}
