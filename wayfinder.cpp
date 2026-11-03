

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
#include <string>
#include <vector>
using namespace std;

const int W = 30, H = 15;
const int INF = 1e9;
const int STRAIGHT = 10, DIAG = 14;  // integer costs (1.0 and ~1.41)

mt19937 rng(random_device{}());

int idx(int x, int y) { return y * W + x; }
bool inBounds(int x, int y) { return x >= 0 && y >= 0 && x < W && y < H; }

struct Edge { int a, b, w; };
struct Result {
    vector<int> path;      // indices from start to goal (empty = no path)
    int cost = 0;
    long long work = 0;    // nodes settled (Dijkstra) / edges examined (Kruskal)
    double micros = 0;
};

vector<char> wall(W * H, 0);
int startIdx = idx(1, 1), goalIdx = idx(W - 2, H - 2);

// neighbours: 8 directions, no cutting corners through walls 
vector<pair<int, int>> neighbours(int i) {  // (neighbour index, cost)
    vector<pair<int, int>> out;
    int x = i % W, y = i / W;
    for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++) {
            if (!dx && !dy) continue;
            int nx = x + dx, ny = y + dy;
            if (!inBounds(nx, ny) || wall[idx(nx, ny)]) continue;
            if (dx && dy && (wall[idx(x + dx, y)] || wall[idx(x, y + dy)])) continue;
            out.push_back({idx(nx, ny), (dx && dy) ? DIAG : STRAIGHT});
        }
    return out;
}

vector<int> rebuild(const vector<int>& prev, int s, int g) {
    vector<int> p;
    if (prev[g] == -1 && g != s) return p;
    for (int at = g; at != -1; at = prev[at]) p.push_back(at);
    reverse(p.begin(), p.end());
    return p;
}

//  Dijkstra
Result dijkstra(int s, int g) {
    auto t0 = chrono::steady_clock::now();
    Result r;
    vector<int> dist(W * H, INF), prev(W * H, -1);
    vector<char> done(W * H, 0);
    using P = pair<int, int>;  // (dist, node)
    priority_queue<P, vector<P>, greater<P>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (done[u]) continue;
        done[u] = 1; r.work++;
        if (u == g) break;  // early exit: the goal is final once popped
        for (auto [v, w] : neighbours(u))
            if (d + w < dist[v]) { dist[v] = d + w; prev[v] = u; pq.push({dist[v], v}); }
    }
    r.path = rebuild(prev, s, g);
    r.cost = dist[g];
    r.micros = chrono::duration<double, micro>(chrono::steady_clock::now() - t0).count();
    return r;
}

//  Kruskal (builds a minimum spanning tree, then walks path madw
struct DSU {
    vector<int> p;
    DSU(int n) : p(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        p[a] = b; return true;
    }
};

Result kruskal(int s, int g) {
    auto t0 = chrono::steady_clock::now();
    Result r;
    // 1. collect every edge in the whole map (each undirected edge once)
    vector<Edge> edges;
    for (int i = 0; i < W * H; i++)
        if (!wall[i])
            for (auto [j, w] : neighbours(i))
                if (j > i) edges.push_back({i, j, w});
    // 2. sort by weight (shuffle first so ties don't favour one direction)
    shuffle(edges.begin(), edges.end(), rng);
    stable_sort(edges.begin(), edges.end(), [](auto& a, auto& b) { return a.w < b.w; });
    // 3. union-find: keep an edge only if it joins two separate components
    DSU dsu(W * H);
    vector<vector<pair<int, int>>> tree(W * H);
    for (auto& e : edges) {
        r.work++;
        if (dsu.unite(e.a, e.b)) {
            tree[e.a].push_back({e.b, e.w});
            tree[e.b].push_back({e.a, e.w});
        }
    }
    // 4. the MST has exactly one route between any two nodes - walk it
    vector<int> prev(W * H, -1), cost(W * H, -1);
    queue<int> q;
    q.push(s); cost[s] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v, w] : tree[u])
            if (cost[v] < 0) { cost[v] = cost[u] + w; prev[v] = u; q.push(v); }
    }
    r.path = rebuild(prev, s, g);
    r.cost = cost[g] < 0 ? INF : cost[g];
    r.micros = chrono::duration<double, micro>(chrono::steady_clock::now() - t0).count();
    return r;
}

// display ting
void draw(const Result& r) {
    vector<char> out(W * H, '.');
    for (int i = 0; i < W * H; i++) if (wall[i]) out[i] = '#';
    for (int i : r.path) out[i] = '*';
    out[startIdx] = 'S'; out[goalIdx] = 'G';
    cout << "\n    ";
    for (int x = 0; x < W; x++) cout << x % 10;
    cout << "\n";
    for (int y = 0; y < H; y++) {
        cout << (y < 10 ? "  " : " ") << y << " ";
        for (int x = 0; x < W; x++) cout << out[idx(x, y)];
        cout << "\n";
    }
}

void randomWalls(int percent) {
    for (int i = 0; i < W * H; i++) wall[i] = (int)(rng() % 100) < percent;
    wall[startIdx] = wall[goalIdx] = 0;
}

void randomGoal() {
    do goalIdx = rng() % (W * H); while (wall[goalIdx] || goalIdx == startIdx);
}

bool readPoint(const string& prompt, int& out) {
    int x, y;
    cout << prompt << " (x y): ";
    if (!(cin >> x >> y)) { cin.clear(); cin.ignore(1 << 20, '\n'); return false; }
    if (!inBounds(x, y)) { cout << "  Out of bounds (0-" << W - 1 << ", 0-" << H - 1 << ").\n"; return false; }
    out = idx(x, y);
    return true;
}

int main() {
    bool useKruskal = false;
    randomWalls(25);
    while (true) {
        Result r = useKruskal ? kruskal(startIdx, goalIdx) : dijkstra(startIdx, goalIdx);
        draw(r);
        cout << "\nAlgorithm : " << (useKruskal ? "Kruskal (MST walk)" : "Dijkstra") << "\n"
             << "Start     : (" << startIdx % W << ", " << startIdx / W << ") = index " << startIdx << "\n"
             << "Goal      : (" << goalIdx % W << ", " << goalIdx / W << ") = index " << goalIdx << "\n";
        if (r.path.empty()) cout << "Path      : NONE (goal is walled off)\n";
        else cout << "Path      : " << r.path.size() << " steps, cost " << r.cost / 10.0 << "\n";
        cout << (useKruskal ? "Edges examined : " : "Nodes settled  : ") << r.work
             << "   Time: " << r.micros << " us\n";

        cout << "\n[1] Set goal   [2] Random goal   [3] Set start   [4] Random obstacles\n"
                "[5] Toggle obstacle   [6] Switch algorithm   [7] Compare both   [q] Quit\n> ";
        string c;
        if (!(cin >> c) || c == "q") break;

        int p;
        if (c == "1") { if (readPoint("Goal", p) && !wall[p] && p != startIdx) goalIdx = p; else cout << "  Invalid goal.\n"; }
        else if (c == "2") randomGoal();
        else if (c == "3") { if (readPoint("Start", p) && !wall[p] && p != goalIdx) startIdx = p; else cout << "  Invalid start.\n"; }
        else if (c == "4") {
            int pct; cout << "Obstacle density % (0-60): ";
            if (cin >> pct) randomWalls(clamp(pct, 0, 60)); else { cin.clear(); cin.ignore(1 << 20, '\n'); }
        }
        else if (c == "5") { if (readPoint("Toggle", p) && p != startIdx && p != goalIdx) wall[p] ^= 1; else cout << "  Invalid cell.\n"; }
        else if (c == "6") useKruskal = !useKruskal;
        else if (c == "7") {
            Result d = dijkstra(startIdx, goalIdx), k = kruskal(startIdx, goalIdx);
            cout << "\n               Dijkstra      Kruskal\n"
                 << "Path cost      " << (d.path.empty() ? -1 : d.cost / 10.0) << "\t\t" << (k.path.empty() ? -1 : k.cost / 10.0) << "\n"
                 << "Work units     " << d.work << "\t\t" << k.work << "\n"
                 << "Time (us)      " << d.micros << "\t\t" << k.micros << "\n";
            cout << "Press Enter..."; cin.ignore(1 << 20, '\n'); cin.get();
        }
    }
}
