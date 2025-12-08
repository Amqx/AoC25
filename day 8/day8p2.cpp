//
// Created by Jonathan on 03-Dec-25.
//

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <compare>
#include <algorithm>

using namespace std;

struct jb {
    const int64_t x;
    const int64_t y;
    const int64_t z;
    const int id;
};

struct measurement {
    int a, b;
    uint64_t dist;

    auto operator<=>(const measurement &other) const {
        return dist <=> other.dist;
    }
    auto operator==(const measurement& other) const {
        return dist == other.dist;
    }
};

struct circuit {
    vector<int> parent;
    vector<int> size;

    int find(const int& x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool join(const int& a, const int& b) {
        int ra = find(a);
        int rb = find(b);

        if (ra == rb) return false;
        if (size[ra] < size[rb]) swap(ra, rb);

        parent[rb] = ra;
        size[ra] += size[rb];
        return true;
    }
};

circuit CreateCircuit(const size_t& size) {
    circuit c;
    c.parent.resize(size);
    c.size.resize(size, 1);

    for (int i = 0; i < size; i++) {
        c.parent[i] = i;
    }
    return c;
}

jb CreatePoint(const string& line, const int& id) {
    const auto pos1 = line.find(',');
    const auto pos2 = line.rfind(',');
    const auto x_ = stoll(line.substr(0, pos1));
    const auto y_ = stoll(line.substr(pos1+1, pos2));
    const auto z_ = stoll(line.substr(pos2+1));
    return jb{x_,y_,z_, id};
}

inline uint64_t pow2(const int64_t& in) {
    return in * in;
}

inline uint64_t Distance(const jb& pt_a, const jb& pt_b) {
    return pow2(pt_a.x - pt_b.x) + pow2(pt_a.y - pt_b.y) + pow2(pt_a.z - pt_b.z);
}

int main() {

    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") {
        file = R"(C:\Users\Jonathan\git\aoc25\day 8\test.txt)";
    }
    else if (file == "input") {
        file = R"(C:\Users\Jonathan\git\aoc25\day 8\input.txt)";
    }
    else {
        cout << "Invalid" << endl;
        return 1;
    }
    double total = 0;
    for (int iteration = 0; iteration < 50; iteration++) {
        const auto t1 = chrono::steady_clock::now();
        if (ifstream input(file); input.is_open()) {

            string line;
            vector<jb> list;
            list.reserve(1000);

            int id = 0;
            while (getline(input, line)) {
                list.emplace_back(CreatePoint(line, id));
                id++;
            }

            vector<measurement> distances;
            for (auto j = 0; j < list.size(); j++) {
                for (auto k = j + 1; k < list.size(); k++) {
                    auto d = Distance(list[j], list[k]);

                    distances.emplace_back(list[j].id, list[k].id, d);
                }
            }
            ranges::sort(distances);

            auto set = CreateCircuit(list.size());

            int last_a, last_b;
            auto components = list.size();
            for (auto& e : distances) {
                if (set.join(e.a, e.b)) {
                    components--;
                    if (components == 1) {
                        last_a = e.a;
                        last_b = e.b;
                        break;
                    }
                }
            }

            const auto t2 = chrono::steady_clock::now();

            uint64_t ans = 1;
            bool af = false;
            bool bf = false;
            for (auto & i : list) {
                if (!af && i.id == last_a) {
                    af = true;
                    ans *= i.x;
                    continue;
                }
                if (!bf && i.id == last_b) {
                    bf = true;
                    ans *= i.x;
                    continue;
                }
                if (af && bf) break;
            }
            cout << "Answer: " << ans << endl;

            total += std::chrono::duration<double, micro>(t2 - t1).count();
        } else {
            cout << "Failed to open file!" << endl;
            return 1;
        }
    }
    cout << "Time (us, avg 50): " << fixed << setprecision(2) << total / 50 << endl; // ~ 58617.46us
    return 0;
}