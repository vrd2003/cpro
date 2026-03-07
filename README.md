# 🔍 C++ DSA Code Analyzer & Visualizer

A **Qt6 desktop application** that analyzes C++ code for Data Structures & Algorithms patterns, estimates Big-O complexity, builds an Abstract Syntax Tree, and visualizes everything in a rich dark-themed GUI.

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C++17-brightgreen)
![Qt](https://img.shields.io/badge/Qt-6.x-41CD52)
![Build](https://img.shields.io/badge/build-CMake%20%2B%20Ninja-orange)

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🌳 **AST Viewer** | Builds and displays a full Abstract Syntax Tree in a tree widget |
| 📈 **Complexity Chart** | Shows 7 growth curves (O(1) → O(n³)), highlights the detected one |
| 💡 **Smart Suggestions** | Priority-queue ranked tips with specific algorithm alternatives |
| 🎨 **Syntax Highlighting** | VS Code-style C++ highlighting (keywords, STL types, DSA functions) |
| 🧠 **Algorithm ID** | Detects 20+ named algorithms (Bubble Sort, BFS, DP, Dijkstra, etc.) |
| 📋 **Example Loader** | 15 built-in examples from O(1) to BFS/DFS/DP/Bit Manipulation |
| 🌙 **Dark Theme** | Catppuccin-inspired dark UI throughout |

---

## 🛠️ Setup & Installation

### Prerequisites

| Tool | Version | Download |
|------|---------|----------|
| **Qt 6** | 6.x (with Qt Charts module) | [qt.io/download-open-source](https://www.qt.io/download-open-source) |
| **CMake** | 3.22+ | Bundled with Qt at `C:\Qt\Tools\CMake_64` |
| **Ninja** | Any | Bundled with Qt at `C:\Qt\Tools\Ninja` |
| **MinGW** | 13+ | Bundled with Qt at `C:\Qt\Tools\mingw1310_64` |

> **Note:** When installing Qt, make sure to select:
> - ✅ Qt 6.x → **MinGW 64-bit** component  
> - ✅ **Qt Charts** under _Additional Libraries_
> - ✅ **CMake** and **Ninja** under _Developer and Designer Tools_

### Clone / Download

```powershell
# If using git
git clone <your-repo-url> c:\projects\cpro
cd c:\projects\cpro

# Or just open c:\projects\cpro in VS Code / Qt Creator
```

---

## 🔨 Build Commands

### Option 1 — One-click batch script (recommended)

```bat
cd c:\projects\cpro
build.bat
```

The script auto-detects `C:\Qt\6.x.x\mingw_64` and uses Qt's bundled CMake + Ninja.

### Option 2 — Manual CMake (PowerShell)

```powershell
# Step 1: Configure
C:\Qt\Tools\CMake_64\bin\cmake.exe `
  -S . -B build `
  -G "Ninja" `
  -DCMAKE_BUILD_TYPE=Release `
  "-DCMAKE_PREFIX_PATH=C:/Qt/6.10.2/mingw_64" `
  "-DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe" `
  "-DCMAKE_C_COMPILER=C:/Qt/Tools/mingw1310_64/bin/gcc.exe" `
  "-DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe"

# Step 2: Build
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build --config Release --parallel

# Step 3: Run
.\build\ComplexityAnalyzer.exe
```

### Option 3 — Qt Creator

1. Open Qt Creator → **File → Open File or Project**
2. Select `c:\projects\cpro\CMakeLists.txt`
3. Configure with the **Desktop Qt 6.x.x MinGW 64-bit** kit
4. Press **Build** (Ctrl+B) then **Run** (Ctrl+R)

### Rebuild after changes

```powershell
# Incremental (only recompiles changed files) — fast
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build --parallel

# Full clean rebuild
Remove-Item -Recurse -Force build
.\build.bat
```

---

## 🚀 Running the App

```bat
build\ComplexityAnalyzer.exe
```

The `build\` folder contains the EXE and all required Qt DLLs. It is fully self-contained — no Qt installation needed on the target machine.

---

## 🧠 Analysis Methodology

The analyzer works in a **6-stage pipeline**:

```
Source Code
    │
    ▼
┌─────────────────────────────────────────────────────┐
│ Stage 1: Tokenization  (TokenProcessor)             │
│  - Splits code into typed tokens line-by-line        │
│  - Recognizes 20+ types: FOR, WHILE, SORT,           │
│    BINARY_SEARCH, QUEUE_OP, PRIORITY_QUEUE,          │
│    HASH_MAP, MEMO_MAP, BIT_OP, FUNC_DEF, etc.       │
│  - Strips comments (// and /* ... */)               │
└────────────────────┬────────────────────────────────┘
                     │ Token stream
          ┌──────────┼──────────┐
          ▼          ▼          ▼
┌──────────────┐ ┌──────────┐ ┌──────────────────────┐
│ Stage 2:     │ │ Stage 3: │ │ Stage 4:             │
│ Loop         │ │ AST      │ │ Pattern              │
│ Detector     │ │ Builder  │ │ Tracker              │
│              │ │          │ │                      │
│ Uses         │ │ Uses     │ │ Uses                 │
│ std::stack   │ │ parent   │ │ unordered_map        │
│ to track     │ │ stack to │ │ to count all         │
│ brace depth  │ │ build    │ │ DSA constructs:      │
│              │ │ tree     │ │ sort, bsearch,       │
│ Output:      │ │          │ │ queue, dp, bit,      │
│ maxDepth,    │ │ Output:  │ │ hash, swap…          │
│ hasSort,     │ │ AST root │ │                      │
│ hasBinSearch │ │ node     │ │                      │
└──────┬───────┘ └────┬─────┘ └──────────┬───────────┘
       │              │                  │
       └──────────────┼──────────────────┘
                      ▼
┌─────────────────────────────────────────────────────┐
│ Stage 5: Complexity Estimator                       │
│  maxDepth=0 + sort  → O(n log n)                   │
│  maxDepth=1         → O(n)                         │
│  maxDepth=1 + sort  → O(n log n)                   │
│  maxDepth=2         → O(n²)                        │
│  maxDepth=2 + sort  → O(n² log n)                  │
│  maxDepth=3+        → O(n³) / O(n^k)               │
│  binary search hint → log n factor added            │
└────────────────────┬────────────────────────────────┘
                     ▼
┌─────────────────────────────────────────────────────┐
│ Stage 6: Algorithm Identifier + Suggestion Engine   │
│                                                     │
│  Identifies primary/secondary algorithm:            │
│   queue+loop    → BFS                              │
│   2 loops+swap  → Bubble Sort                      │
│   recursion     → DFS / Divide & Conquer           │
│   dp[]+loop     → DP Tabulation                    │
│   priority_queue → Greedy / Dijkstra               │
│   unordered_map  → Hashing                         │
│                                                     │
│  Suggestions ranked by std::priority_queue          │
│  (severity 5=critical → 1=info)                    │
└─────────────────────────────────────────────────────┘
                     │
                     ▼
            GUI (Qt6 MainWindow)
         ┌──────────┬────────────┐
         │ AST Tree │   Chart    │
         │ QTree    │ QChartView │
         │ Widget   │ (7 curves) │
         └──────────┴──────┬─────┘
                           │
                    QListWidget
                   (Suggestions)
```

### Big-O Estimation Rules

| Loop Depth | No Sort/Search | With `sort()` | With `binary_search` |
|:---:|---|---|---|
| 0 | **O(1)** | **O(n log n)** | **O(log n)** |
| 1 | **O(n)** | **O(n log n)** | **O(n log n)** |
| 2 | **O(n²)** | **O(n² log n)** | **O(n²)** |
| 3 | **O(n³)** | **O(n³ log n)** | **O(n³)** |
| k | **O(n^k)** | **O(n^k log n)** | **O(n^k)** |

### Detected Algorithms

| Algorithm | Detection Signal |
|-----------|----------------|
| Bubble Sort | 2 nested loops + `swap()` |
| Selection Sort | 2 nested loops, no swap |
| Insertion Sort | 1 `while` loop |
| std::sort | `sort()` / `stable_sort()` call |
| Quick Sort | recursion + `swap()` |
| Merge / D&C | recursion, no swap |
| Linear Search | `std::find()` in loop |
| Binary Search | `binary_search()` / `lower_bound()` |
| BFS | `queue` + while loop |
| DFS (recursive) | recursion + adjacency traversal |
| DFS (iterative) | `stack` + loop |
| DP Memoization | `dp[i]`/`memo[]` + recursion |
| DP Tabulation | `dp[i]`/`memo[]` + loop |
| Greedy | `priority_queue` |
| Hashing | `unordered_map` / `unordered_set` |
| Bit Manipulation | `>>1`, `&1`, `__builtin_popcount` |

---

## 📁 Project Structure

```
cpro/
├── CMakeLists.txt               ← Qt6 + Ninja build config (C++17)
├── main.cpp                     ← Qt entry point + dark Fusion palette
├── build.bat                    ← One-click build script
├── README.md                    ← This file
│
├── src/                         ← Pure C++ analyzer engine (zero Qt)
│   ├── ASTNode.h                ← Tree node: label, depth, line, children
│   ├── TokenProcessor.h/.cpp    ← Lexer: 20+ token types
│   ├── LoopDetector.h/.cpp      ← std::stack depth tracking
│   ├── ASTBuilder.h/.cpp        ← Parent-stack tree construction
│   ├── PatternTracker.h/.cpp    ← unordered_map pattern counts + AlgorithmKind
│   ├── ComplexityEstimator.h/.cpp ← Big-O rule engine
│   ├── SuggestionEngine.h/.cpp  ← priority_queue ranked suggestions
│   └── Analyzer.h/.cpp          ← Facade: wires all sub-engines
│
└── gui/                         ← Qt6 presentation layer
    ├── SyntaxHighlighter.h/.cpp ← QSyntaxHighlighter (VS Code colors)
    ├── ChartHelper.h/.cpp       ← QChart: 7 growth curves
    └── MainWindow.h/.cpp        ← Main window, dark stylesheet
```

### Internal DSA Structures Used

| Structure | File | Role |
|-----------|------|------|
| `std::stack<bool>` | `LoopDetector` | Loop brace nesting depth |
| `std::stack<shared_ptr<ASTNode>>` | `ASTBuilder` | Parent-tracking during tree build |
| `std::unordered_map<string,int>` | `PatternTracker` | Pattern occurrence counts |
| `std::priority_queue<Suggestion>` | `SuggestionEngine` | Severity-ordered suggestions |
| `std::shared_ptr<ASTNode>` | All | Recursive tree node ownership |
| `std::vector<Token>` | `TokenProcessor` | Flat token stream |

---

## 🧪 Test Cases

| Input | Expected | Algorithm Detected |
|-------|----------|--------------------|
| `int x = arr[0];` | O(1) | — |
| `for(i<n) { }` | O(n) | Linear |
| `sort(v.begin(),v.end());` | O(n log n) | std::sort |
| `binary_search(...)` | O(n log n) | Binary Search |
| Two nested `for` loops | O(n²) | Bubble/Selection Sort |
| Two nested + `sort()` | O(n² log n) | — |
| Three nested `for` loops | O(n³) | — |
| `queue` + `while` loop | O(V+E) | BFS |
| `dp[i] = dp[i-1]+...` | O(n) | DP Tabulation |
| `unordered_map` lookup | O(n) | Hashing |
| `n >>= 1; n & 1` | O(log n) | Bit Manipulation |

---

## 📝 License

MIT — free for personal and educational use.
