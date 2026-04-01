#pragma once
#include <vector>
#include <utility>
#include <QString>

// ── Built-in DSA code examples ────────────────────────────────────────────────
// Extracted from MainWindow.cpp for maintainability.
// Loaded into the example combo box by MainWindow::buildUi().

inline const std::vector<std::pair<QString, QString>> k_examples = {
    {"-- Load Example --", ""},

    // ── Constant / Linear ────────────────────────────────────────────────────
    {"O(1) — Constant access",
        "int getFirst(int arr[], int n) {\n"
        "    return arr[0];\n"
        "}"},
    {"O(n) — Linear search",
        "int linearSearch(int arr[], int n, int x) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        if (arr[i] == x) {\n"
        "            return i;\n"
        "        }\n"
        "    }\n"
        "    return -1;\n"
        "}"},

    // ── Logarithmic ──────────────────────────────────────────────────────────
    {"O(log n) — Binary search",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "bool found(vector<int>& v, int x) {\n"
        "    sort(v.begin(), v.end());\n"
        "    return binary_search(v.begin(), v.end(), x);\n"
        "}"},

    // ── NlogN ────────────────────────────────────────────────────────────────
    {"O(n log n) — sort",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void sortArray(vector<int>& v) {\n"
        "    sort(v.begin(), v.end());\n"
        "}"},

    // ── Quadratic ────────────────────────────────────────────────────────────
    {"O(n²) — Bubble Sort",
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void bubbleSort(int arr[], int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n - i - 1; j++) {\n"
        "            if (arr[j] > arr[j+1]) {\n"
        "                swap(arr[j], arr[j+1]);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},
    {"O(n²) — Selection Sort",
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void selectionSort(int arr[], int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        int minIdx = i;\n"
        "        for (int j = i + 1; j < n; j++) {\n"
        "            if (arr[j] < arr[minIdx]) {\n"
        "                minIdx = j;\n"
        "            }\n"
        "        }\n"
        "        swap(arr[i], arr[minIdx]);\n"
        "    }\n"
        "}"},

    // ── Higher order ─────────────────────────────────────────────────────────
    {"O(n² log n) — Nested + sort",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "void process(vector<int>& v, int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n; j++) {\n"
        "            sort(v.begin(), v.end());\n"
        "        }\n"
        "    }\n"
        "}"},
    {"O(n³) — Triple nested",
        "void tripleLoop(int n) {\n"
        "    for (int i = 0; i < n; i++) {\n"
        "        for (int j = 0; j < n; j++) {\n"
        "            for (int k = 0; k < n; k++) {\n"
        "                // O(n³)\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},

    // ── Graph ────────────────────────────────────────────────────────────────
    {"BFS — O(V+E)",
        "#include <vector>\n"
        "#include <queue>\n"
        "using namespace std;\n\n"
        "void bfs(int start, vector<vector<int>>& adj, int n) {\n"
        "    vector<bool> visited(n, false);\n"
        "    queue<int> q;\n"
        "    q.push(start);\n"
        "    visited[start] = true;\n"
        "    while (!q.empty()) {\n"
        "        int node = q.front();\n"
        "        q.pop();\n"
        "        for (int nb : adj[node]) {\n"
        "            if (!visited[nb]) {\n"
        "                visited[nb] = true;\n"
        "                q.push(nb);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},
    {"DFS Recursive — O(V+E)",
        "#include <vector>\n"
        "using namespace std;\n\n"
        "void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited) {\n"
        "    visited[node] = true;\n"
        "    for (int nb : adj[node]) {\n"
        "        if (!visited[nb]) {\n"
        "            dfs(nb, adj, visited);\n"
        "        }\n"
        "    }\n"
        "}"},

    // ── Dynamic Programming ──────────────────────────────────────────────────
    {"DP — Fibonacci (Tabulation)",
        "#include <vector>\n"
        "using namespace std;\n\n"
        "int fib(int n) {\n"
        "    vector<int> dp(n + 1, 0);\n"
        "    dp[0] = 0;\n"
        "    dp[1] = 1;\n"
        "    for (int i = 2; i <= n; i++) {\n"
        "        dp[i] = dp[i-1] + dp[i-2];\n"
        "    }\n"
        "    return dp[n];\n"
        "}"},
    {"DP — 0/1 Knapsack O(nW)",
        "#include <vector>\n"
        "#include <algorithm>\n"
        "using namespace std;\n\n"
        "int knapsack(int W, int wt[], int val[], int n) {\n"
        "    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));\n"
        "    for (int i = 1; i <= n; i++) {\n"
        "        for (int w = 0; w <= W; w++) {\n"
        "            dp[i][w] = dp[i-1][w];\n"
        "            if (wt[i-1] <= w) {\n"
        "                dp[i][w] = max(dp[i][w], dp[i-1][w-wt[i-1]] + val[i-1]);\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "    return dp[n][W];\n"
        "}"},

    // ── Hashing ──────────────────────────────────────────────────────────────
    {"Hashing — Two Sum O(n)",
        "#include <vector>\n"
        "#include <unordered_map>\n"
        "using namespace std;\n\n"
        "pair<int,int> twoSum(vector<int>& nums, int target) {\n"
        "    unordered_map<int,int> mp;\n"
        "    for (int i = 0; i < (int)nums.size(); i++) {\n"
        "        if (mp.count(target - nums[i])) {\n"
        "            return {mp[target-nums[i]], i};\n"
        "        }\n"
        "        mp[nums[i]] = i;\n"
        "    }\n"
        "    return {-1,-1};\n"
        "}"},

    // ── Greedy / Heap ─────────────────────────────────────────────────────────
    {"Greedy — Min Heap (Dijkstra style)",
        "#include <vector>\n"
        "#include <queue>\n"
        "using namespace std;\n\n"
        "void dijkstra(int src, vector<pair<int,int>> adj[], int n) {\n"
        "    vector<int> dist(n, INT_MAX);\n"
        "    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;\n"
        "    dist[src] = 0;\n"
        "    pq.push({0, src});\n"
        "    while (!pq.empty()) {\n"
        "        auto [d, u] = pq.top();\n"
        "        pq.pop();\n"
        "        for (auto [w, v] : adj[u]) {\n"
        "            if (dist[u] + w < dist[v]) {\n"
        "                dist[v] = dist[u] + w;\n"
        "                pq.push({dist[v], v});\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}"},

    // ── Bit Manipulation ─────────────────────────────────────────────────────
    {"Bit Manipulation — Count set bits",
        "int countBits(int n) {\n"
        "    int count = 0;\n"
        "    while (n > 0) {\n"
        "        count += n & 1;\n"
        "        n >>= 1;\n"
        "    }\n"
        "    return count;\n"
        "}"},
};
