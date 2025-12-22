//
// Created by Jonathan on 03-Dec-25.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct Point {
    int r;
    int c;

    bool operator<(const Point &other) const {
        if (r != other.r) return r < other.r;
        return c < other.c;
    }

    bool operator==(const Point &other) const {
        return r == other.r && c == other.c;
    }
};

struct Variant {
    int height;
    int width;
    vector<Point> cells;
};

struct Present {
    int id{};
    int area{};
    vector<Variant> variants;
};

void normalize(vector<Point> &pts) {
    if (pts.empty()) return;
    int min_r = pts[0].r;
    int min_c = pts[0].c;
    for (const auto &p: pts) {
        if (p.r < min_r) min_r = p.r;
        if (p.c < min_c) min_c = p.c;
    }
    for (auto &p: pts) {
        p.r -= min_r;
        p.c -= min_c;
    }
    sort(pts.begin(), pts.end());
}

vector<Variant> CreateVariants(const vector<string> &shape) {
    vector<Point> base_points;
    for (int r = 0; r < shape.size(); r++) {
        for (int c = 0; c < shape[r].size(); c++) {
            if (shape[r][c] == '#') {
                base_points.push_back({r, c});
            }
        }
    }
    set<vector<Point> > configs;
    vector<Variant> variants;

    vector<Point> current = base_points;
    for (int flip = 0; flip < 2; flip++) {
        for (int rot = 0; rot < 4; rot++) {
            normalize(current);
            if (!configs.contains(current)) {
                configs.insert(current);

                int max_r = 0, max_c = 0;
                for (auto &p: current) {
                    max_r = max(max_r, p.r);
                    max_c = max(max_c, p.c);
                }
                variants.push_back({max_r + 1, max_c + 1, current});
            }

            for (auto &p: current) {
                const int temp = p.r;
                p.r = p.c;
                p.c = -temp;
            }
        }
        current = base_points;
        for (auto &p: current) {
            p.c = -p.c;
        }
        base_points = current;
    }
    return variants;
}

bool CanPlace(const vector<vector<bool> > &board, const uint64_t r, const uint64_t c, const Variant &var) {
    const auto H = board.size();
    const auto W = board[0].size();
    if (r + var.height > H || c + var.width > W) return false;

    if (!ranges::all_of(var.cells, [&](const auto &p) {
        return !board[r + p.r][c + p.c];
    }))
        return false;

    return true;
}

void Toggle(vector<vector<bool> > &board, const uint64_t r, const uint64_t c, const Variant &var, const bool val) {
    for (const auto &p: var.cells) {
        board[r + p.r][c + p.c] = val;
    }
}

bool solve(vector<vector<bool> > &board, const vector<int> &required, const int idx, const int last_pos,
           const map<int, Present> &shapes) {
    if (idx >= required.size()) return true;

    const int shape_id = required[idx];
    const Present &info = shapes.at(shape_id);

    const auto H = board.size();
    const auto W = board[0].size();

    int start_pos = 0;
    if (idx > 0 && required[idx] == required[idx - 1]) {
        start_pos = last_pos;
    }

    for (int pos = start_pos; pos < H * W; pos++) {
        const auto r = pos / W;
        const auto c = pos % W;

        for (const auto &var: info.variants) {
            if (CanPlace(board, r, c, var)) {
                Toggle(board, r, c, var, true);

                if (solve(board, required, idx + 1, pos, shapes)) {
                    return true;
                }

                Toggle(board, r, c, var, false);
            }
        }
    }
    return false;
}

int main() {
    cout << "Which input (test/ input): " << endl;
    string file;
    cin >> file;
    if (file == "test") file = R"(C:\Users\Jonathan\git\aoc25\day 12\test.txt)";
    else if (file == "input") file = R"(C:\Users\Jonathan\git\aoc25\day 12\input.txt)";
    else {
        cout << "Invalid" << endl;
        return 1;
    }

    if (ifstream input(file); input.is_open()) {
        string line;
        map<int, Present> shapes;
        vector<string> current_shape_lines;
        int id = -1;
        vector<string> query_lines;

        while (getline(input, line)) {
            if (line.empty()) continue;

            size_t colon_pos = line.find(':');
            size_t x_pos = line.find('x');

            if (x_pos != string::npos && colon_pos != string::npos && x_pos < colon_pos) {
                query_lines.push_back(line);
                while (getline(input, line)) {
                    if (!line.empty()) query_lines.push_back(line);
                }
                break;
            }

            if (colon_pos != string::npos) {
                if (id != -1) {
                    shapes[id] = {id, 0, CreateVariants(current_shape_lines)};
                    int area = 0;
                    for (const auto &s: current_shape_lines) {
                        for (char c: s) if (c == '#') area++;
                    }
                    shapes[id].area = area;
                }
                id = stoi(line.substr(0, colon_pos));
                current_shape_lines.clear();
            } else {
                current_shape_lines.push_back(line);
            }
        }
        if (id != -1) {
            shapes[id] = {id, 0, CreateVariants(current_shape_lines)};
            int area = 0;
            for (const auto &s: current_shape_lines) {
                for (const auto &c: s) {
                    if (c == '#') area++;
                }
            }
            shapes[id].area = area;
        }
        int valid = 0;
        for (const string &q_line: query_lines) {
            auto x_pos = q_line.find('x');
            auto colon_pos = q_line.find(':');

            int width = stoi(q_line.substr(0, x_pos));
            int height = stoi(q_line.substr(x_pos + 1, colon_pos - (x_pos + 1)));

            stringstream ss(q_line.substr(colon_pos + 1));
            vector<int> required;
            int count;
            int shape_idx = 0;
            int total_area = 0;

            while (ss >> count) {
                for (int i = 0; i < count; i++) {
                    required.push_back(shape_idx);
                    if (shapes.contains(shape_idx)) {
                        total_area += shapes[shape_idx].area;
                    }
                }
                shape_idx++;
            }

            ranges::sort(required, [&](const int a, const int b) {
                if (shapes[a].area != shapes[b].area) {
                    return shapes[a].area > shapes[b].area;
                }
                return a < b;
            });

            if (total_area > width * height) continue;

            if (vector board(height, vector<bool>(width, false)); solve(board, required, 0, 0, shapes)) valid++;
        }

        cout << "Total: " << valid << endl; // i'm not even going to bother timing this one i'm too tired atp

        return 0;
    }
    cout << "Failed to open file" << endl;
    return 1;
}
