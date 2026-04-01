---
description: Analyze uploaded C++ code for time/space complexity and modularity — suggestions only
---

# Code Analysis Workflow

Triggered whenever the user uploads or pastes C++ code for review.
This is a MANUAL, SUGGESTION-ONLY workflow. You MUST NOT make any code changes automatically.
Every output is a recommendation for the user to accept, modify, or reject.

---

## Step 1 — Read All Files

Use `view_file` to read every uploaded or modified `.cpp` / `.h` file in full.
Do not skip any file. If directories are mentioned, list them first with `list_dir`.

---

## Step 2 — Per-Function Complexity Analysis

For **every** function / method found, produce an entry in this exact table format:

| File | Function / Method | Time Complexity | Space Complexity | Justification |
|------|-------------------|-----------------|------------------|---------------|
| `file.cpp` | `functionName(params)` | O(?) | O(?) | One-sentence reason |

### Rules for accurate complexity derivation

**Time Complexity**

- A **single flat loop** over n elements → **O(n)**
- **Two nested loops** each iterating n → **O(n²)**; if inner bound differs → multiply bounds
- **Divide-and-conquer recursion** (halving each time, O(1) work per level) → **O(log n)**
- `std::sort` / `std::stable_sort` on n elements → **O(n log n)**
- `std::binary_search` / `lower_bound` / `upper_bound` → **O(log n)** (requires sorted input)
- `std::find` / linear scan → **O(n)**
- BFS / DFS on graph with V vertices, E edges → **O(V + E)**
- DP table of size n×W filled once → **O(n·W)**
- Recursion with branching factor b and depth d (no memoization) → **O(b^d)**
- Heap push/pop via `priority_queue` → **O(log n)** per operation; over n elements → **O(n log n)**
- `unordered_map` / `unordered_set` lookup/insert → **O(1)** amortized
- `map` / `set` lookup/insert → **O(log n)**
- Bit operations (`>>`, `&`, `^`) → **O(1)** per operation; loop using `n >>= 1` → **O(log n)**
- Nested loops where inner calls `sort` → multiply: O(n²) × O(n log n) = **O(n³ log n)**

**Space Complexity**

- No auxiliary allocation → **O(1)**
- Single auxiliary array of size n → **O(n)**
- 2D DP table n×m → **O(n·m)**
- Recursive call stack depth d → **O(d)** additional stack space
- BFS queue can hold up to V nodes → **O(V)**
- Hash map storing n entries → **O(n)**

**Dominant-term rule**: report only the highest-order term; drop constants and lower-order terms.
**Worst-case default**: always report worst-case unless the function is explicitly average/best-case only.

---

## Step 3 — Overall (Whole-File / Whole-Program) Complexity

State the overall time and space complexity of the program/module as a whole:

```
Overall Time Complexity  : O(?)
Overall Space Complexity : O(?)
Dominant bottleneck      : <function name and why>
```

---

## Step 4 — Modularity Review

Evaluate each class and free function against these criteria:

| Criterion | Question to ask | Rating (1–5) |
|-----------|-----------------|--------------|
| **Single Responsibility** | Does this class/function do only ONE thing? | |
| **Cohesion** | Are all members/methods closely related? | |
| **Coupling** | Does it depend on too many other classes directly? | |
| **Interface clarity** | Is the public API minimal and self-explanatory? | |
| **Reusability** | Could this component be used in another project unchanged? | |
| **Header hygiene** | Are headers free of unnecessary `using namespace` in global scope? | |
| **Separation of concerns** | Is GUI logic separated from business/analysis logic? | |

Rating scale: 1 = Poor, 3 = Acceptable, 5 = Excellent.

For each finding rated < 4, provide a **specific, actionable suggestion**.

---

## Step 5 — Specific Project Observations (cpro)

When analyzing this project specifically, always check:

### `src/TokenProcessor`
- `classifyWord()` — O(1) per token (sequential if-chain); `tokenize()` — O(L·W) where L = lines, W = words/line
- Watch for: `static` keyword vectors allocated inside loops (e.g., `kKeywords` inside `tokenize`)
- Modularity note: the first-pass block inside `tokenize` is empty (dead code) → suggest removing

### `src/ASTBuilder`
- `build()` — O(T) where T = number of tokens; stack operations O(1)
- Space: O(F) where F = number of function/control-flow nodes (AST nodes allocated)
- Modularity note: `makeNode` lambda defined per-call inside a loop; fine for readability but could be a private helper

### `src/Analyzer`
- `analyze()` — O(T) total (delegates to TokenProcessor + ASTBuilder)
- Space: O(N) for the AST tree

### `gui/MainWindow`
- `buildUi()` — pure UI setup, O(E) where E = number of UI example entries
- `onAnalyze()` — sends async HTTP request; complexity is network-bound + O(L·W) for local analysis
- `onGeminiResponse()` — O(S) where S = number of suggestion items in JSON
- `populateAST()` — O(N) recursive tree traversal where N = AST nodes
- `updateChart()` — O(1) (replaces chart pointer)
- `nodeIcon()` — O(1) (fixed label comparisons)
- Modularity concern: API key is hardcoded as a `QString` literal — suggest moving to config/env
- Modularity concern: `onGeminiResponse` does JSON parsing + UI update + complexity classification in one 100-line slot — suggest splitting into `parseGeminiJson()`, `applyComplexityResult()` private helpers
- Modularity concern: `k_examples` and `k_stylesheet` are file-scoped statics in `MainWindow.cpp` — suggest extracting to separate resource files

### `gui/SyntaxHighlighter`
- Check that highlighting rules are data-driven (a `QVector<HighlightRule>`) not one giant switch

### `gui/ChartHelper`
- `buildChart()` should be stateless; verify it takes a `ComplexityClass` and returns a new `QChart*` with no side effects

---

## Step 6 — Final Suggestion Summary

List all suggestions in priority order (highest impact first):

```
SUGGESTIONS (manual review only — DO NOT auto-apply):

[CRITICAL]   <description>
[HIGH]       <description>
[MEDIUM]     <description>
[LOW]        <description>
[STYLE]      <description>
```

Severity definitions:
- **CRITICAL** — correctness bug, security issue, or crash risk
- **HIGH**     — significant performance problem or major SRP violation
- **MEDIUM**   — notable modularity/maintainability issue
- **LOW**      — minor inefficiency or style inconsistency
- **STYLE**    — purely cosmetic / naming convention

---

## Step 7 — Constraints (ALWAYS ENFORCE)

1. **Never apply any suggestion automatically.** Output is read-only advisory.
2. **Never skip a function** — every function must appear in the complexity table.
3. **Justify every complexity** with one-sentence reasoning (no bare O(n) without explanation).
4. **If complexity is ambiguous** (e.g., depends on input distribution), state the worst-case and note the caveat.
5. **Do not hallucinate algorithms** — derive complexity from the actual code tokens/structure visible in the files.
6. **Report space complexity separately** from time — never conflate them.
