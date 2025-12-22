//
// Created by Jonathan on 03-Dec-25.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <limits>
#include <numeric>
#include <algorithm>
#include <functional>

using namespace std;

struct Machine {
    vector<int> target;
    vector<vector<int> > buttons;
};

struct Rational {
    int64_t n = 0;
    int64_t d = 1;

    Rational() = default;

    explicit Rational(const int64_t num) : n(num), d(1) {
    }

    Rational(const int64_t num, const int64_t den) : n(num), d(den) { norm(); }

    static int64_t igcd(int64_t a, int64_t b) {
        if (a < 0) a = -a;
        if (b < 0) b = -b;
        return gcd(a, b);
    }

    void norm() {
        if (d < 0) {
            d = -d;
            n = -n;
        }
        if (n == 0) {
            d = 1;
            return;
        }
        const int64_t g = igcd(n, d);
        n /= g;
        d /= g;
    }

    [[nodiscard]] bool isZero() const {
        return n == 0;
    }

    [[nodiscard]] bool isInt() const {
        return d == 1;
    }

    friend Rational operator+(const Rational &a, const Rational &b) {
        const __int128 nn = static_cast<__int128>(a.n) * b.d + static_cast<__int128>(b.n) * a.d;
        const __int128 dd = static_cast<__int128>(a.d) * b.d;
        Rational r(static_cast<int64_t>(nn), static_cast<int64_t>(dd));
        r.norm();
        return r;
    }

    friend Rational operator-(const Rational &a, const Rational &b) {
        const __int128 nn = static_cast<__int128>(a.n) * b.d - static_cast<__int128>(b.n) * a.d;
        const __int128 dd = static_cast<__int128>(a.d) * b.d;
        Rational r(static_cast<int64_t>(nn), static_cast<int64_t>(dd));
        r.norm();
        return r;
    }

    friend Rational operator*(const Rational &a, const Rational &b) {
        const __int128 nn = static_cast<__int128>(a.n) * b.n;
        const __int128 dd = static_cast<__int128>(a.d) * b.d;
        Rational r(static_cast<int64_t>(nn), static_cast<int64_t>(dd));
        r.norm();
        return r;
    }

    friend Rational operator/(const Rational &a, const Rational &b) {
        // assume b != 0
        const __int128 nn = static_cast<__int128>(a.n) * b.d;
        const __int128 dd = static_cast<__int128>(a.d) * b.n;
        Rational r(static_cast<int64_t>(nn), static_cast<int64_t>(dd));
        r.norm();
        return r;
    }

    friend Rational operator*(const Rational &a, int64_t k) {
        const __int128 nn = static_cast<__int128>(a.n) * k;
        Rational r(static_cast<int64_t>(nn), a.d);
        r.norm();
        return r;
    }
};

vector<vector<int> > ProcessButtons(string line) {
    vector<vector<int> > buttons;
    auto sep = line.find(')');
    while (sep != string::npos) {
        string inside = line.substr(1, sep - 1);
        vector<int> btn;
        int num = 0;
        bool in_number = false;
        for (const auto &c: inside) {
            if (c >= '0' && c <= '9') {
                num = num * 10 + (c - '0');
                in_number = true;
            } else {
                if (in_number) {
                    btn.push_back(num);
                    num = 0;
                    in_number = false;
                }
            }
        }
        if (in_number) btn.push_back(num);

        buttons.push_back(btn);

        if (sep + 2 > line.length()) break;
        line = line.substr(sep + 2);
        sep = line.find(')');
    }
    return buttons;
}

vector<int> ProcessTarget(const string &line) {
    vector<int> r;
    int num = 0;
    bool in_number = false;

    for (const char &c: line) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            in_number = true;
        } else {
            if (in_number) {
                r.push_back(num);
                num = 0;
                in_number = false;
            }
        }
    }
    if (in_number) r.push_back(num);
    return r;
}

Machine CreateMachine(const string &line) {
    const auto req_end = line.find(']');
    const auto buttons_end = line.rfind('{');
    Machine r;
    r.buttons = ProcessButtons(line.substr(req_end + 2, buttons_end - req_end - 2 - 1));
    r.target = ProcessTarget(line.substr(buttons_end + 1, line.size() - buttons_end - 2));
    return r;
}

int SolveMachineCGPT(const Machine &m) {
    const auto &target = m.target;
    const auto &buttons = m.buttons;

    const int M = static_cast<int>(target.size());
    const int N = static_cast<int>(buttons.size());

    vector ub(N, 0);
    for (int j = 0; j < N; j++) {
        int mn = numeric_limits<int>::max();
        for (const int idx: buttons[j]) mn = min(mn, target[idx]);
        ub[j] = (mn == numeric_limits<int>::max()) ? 0 : mn;
    }

    vector A(M, vector(N + 1, Rational(0)));
    for (int i = 0; i < M; i++) {
        A[i][N] = Rational(target[i]);
    }
    for (int j = 0; j < N; j++) {
        for (int idx: buttons[j]) {
            A[idx][j] = Rational(1);
        }
    }

    vector pivotRowOfCol(N, -1);
    int row = 0;
    for (int col = 0; col < N && row < M; col++) {
        int piv = -1;
        for (int r = row; r < M; r++) {
            if (!A[r][col].isZero()) {
                piv = r;
                break;
            }
        }
        if (piv == -1) continue;

        swap(A[row], A[piv]);
        Rational p = A[row][col];

        for (int c = col; c <= N; c++) {
            A[row][c] = A[row][c] / p;
        }

        for (int r = 0; r < M; r++) {
            if (r == row) continue;
            if (A[r][col].isZero()) continue;
            Rational f = A[r][col];
            for (int c = col; c <= N; c++) {
                A[r][c] = A[r][c] - f * A[row][c];
            }
        }

        pivotRowOfCol[col] = row;
        row++;
    }

    for (int r = 0; r < M; r++) {
        bool all0 = true;
        for (int c = 0; c < N; c++) {
            if (!A[r][c].isZero()) {
                all0 = false;
                break;
            }
        }
        if (all0 && !A[r][N].isZero()) return -1;
    }

    vector<int> freeCols;
    freeCols.reserve(N);
    for (int c = 0; c < N; c++) {
        if (pivotRowOfCol[c] == -1) freeCols.push_back(c);
    }

    vector<int64_t> x(N, 0);
    auto tryComputeAll = [&](const vector<int64_t> &xFree) -> bool {
        for (size_t k = 0; k < freeCols.size(); k++) {
            int fc = freeCols[k];
            x[fc] = xFree[k];
            if (x[fc] < 0 || x[fc] > ub[fc]) return false;
        }

        for (int pc = 0; pc < N; pc++) {
            int pr = pivotRowOfCol[pc];
            if (pr == -1) continue;

            Rational val = A[pr][N];
            for (size_t k = 0; k < freeCols.size(); k++) {
                int fc = freeCols[k];
                if (!A[pr][fc].isZero()) {
                    val = val - (A[pr][fc] * xFree[k]);
                }
            }
            if (!val.isInt()) return false;

            int64_t iv = val.n;
            if (iv < 0 || iv > ub[pc]) return false;
            x[pc] = iv;
        }

        return true;
    };

    if (freeCols.empty()) {
        vector<int64_t> dummy;
        if (!tryComputeAll(dummy)) {
            return -1;
        }

        uint64_t sum = 0;
        for (int j = 0; j < N; j++) sum += x[j];
        return static_cast<int>(sum);
    }

    ranges::sort(freeCols, [&](const int a, const int b) {
        return ub[a] < ub[b];
    });

    const int D = static_cast<int>(freeCols.size());
    vector<int64_t> xFree(D, 0);
    int64_t best = numeric_limits<int64_t>::max();
    function<void(int, int64_t)> dfs = [&](int k, int64_t partialSum) {
        if (partialSum >= best) return;

        if (k == D) {
            if (!tryComputeAll(xFree)) return;
            int64_t total = 0;
            for (int j = 0; j < N; j++) {
                total += x[j];
            }
            best = min(best, total);
            return;
        }
        int fc = freeCols[k];
        for (int64_t v = 0; v <= ub[fc]; v++) {
            xFree[k] = v;
            dfs(k + 1, partialSum + v);
        }
    };

    dfs(0, 0);
    return (best == numeric_limits<int64_t>::max()) ? -1 : static_cast<int>(best);
}

void PrintMachine(const Machine &m) {
    cout << "Machine: {";
    for (size_t i = 0; i < m.target.size(); ++i) {
        if (i) cout << ",";
        cout << m.target[i];
    }
    cout << "}\nButtons: ";
    for (const auto &b: m.buttons) {
        cout << "(";
        for (size_t i = 0; i < b.size(); ++i) {
            if (i) cout << ",";
            cout << b[i];
        }
        cout << ") ";
    }
    cout << "\n";
}


static double ns_to_ms(const uint64_t ns) {
    return static_cast<double>(ns) / 1'000'000.0;
}

uint64_t multisolve(const vector<Machine> &machines) {
    using clock = std::chrono::steady_clock;

    const unsigned t = max(1u, thread::hardware_concurrency());
    const unsigned workers_count = max(1u, (t > 2) ? (t - 2) : 1u);

    atomic_size_t next{0};
    vector<uint64_t> results(machines.size(), 0);
    vector<uint64_t> solve_ns(machines.size(), 0); // per-machine duration in ns

    vector<jthread> workers;
    workers.reserve(workers_count);

    for (unsigned i = 0; i < workers_count; i++) {
        workers.emplace_back([&]() {
            while (true) {
                const size_t j = next.fetch_add(1, memory_order_relaxed);
                if (j >= machines.size()) break;

                const auto &m = machines[j];

                const auto start = clock::now();
                const uint64_t res = SolveMachineCGPT(m);
                const auto end = clock::now();

                const uint64_t dur =
                        static_cast<uint64_t>(
                            chrono::duration_cast<chrono::nanoseconds>(end - start).count());

                results[j] = res;
                solve_ns[j] = dur;
            }
        });
    }
    for (auto &w: workers) w.join();

    uint64_t sum = 0;
    uint64_t total_ns = 0;

    uint64_t min_ns = (machines.empty() ? 0 : solve_ns[0]);
    uint64_t max_ns = 0;

    for (size_t i = 0; i < machines.size(); ++i) {
        sum += results[i];
        total_ns += solve_ns[i];
        min_ns = std::min(min_ns, solve_ns[i]);
        max_ns = std::max(max_ns, solve_ns[i]);
    }

    cout << "Total solve time: " << ns_to_ms(total_ns) << " ms\n\n";

    return total_ns;
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
    if (ifstream input(file); input.is_open()) {
        string line;
        vector<Machine> machines;
        while (getline(input, line)) {
            machines.push_back(CreateMachine(line));
        }

        __int128 total = 0;
        for (int iter = 0; iter < 50; iter++) {
            total += multisolve(machines);
        }

        cout << "Total average: " << ns_to_ms(static_cast<uint64_t>(total / 50)) << "ms" << endl; // ~ 183.612ms
    } else {
        cout << "Failed to open file!" << endl;
        return 1;
    }
    return 0;
}
