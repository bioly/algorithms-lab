# Algorithms Lab

A learning spot for solving tricky algorithm problems in modern C++, the kind asked in technical interviews at companies like Google.

The goal is not to memorize answers. It is to learn **how to think** about a problem:

1. **Clarify** the problem before touching code.
2. State the **brute force** and why it is too slow.
3. Find the **key insight** that makes it fast.
4. **Watch** the algorithm run, step by step, on a small example.
5. Write **clean code**, with comments that explain the *why*.
6. State the **complexity** and prove it.
7. **Test** the edge cases, and cross-check against a brute force.

Every challenge is a small, self-contained folder with a README that walks through those steps, a commented solution, GoogleTest tests, and a **trace demo** that prints the algorithm's state at every step in the terminal.

## Challenges

All challenges use a farmer story to keep the pictures simple.

| # | Challenge | Pattern | Complexity |
|---|---|---|---|
| 01 | [Maximal Square](challenges/01_maximal_square/README.md) | Dynamic programming on a grid | O(rows x cols) time, O(cols) space |
| 02 | [Count the Farmer's Fields](challenges/02_count_fields/README.md) | Flood fill, connected components | O(rows x cols) |
| 03 | [The Longest Clean Row](challenges/03_longest_unique_run/README.md) | Sliding window | O(n) time, O(1) space |
| 04 | [Merging the Planting Schedules](challenges/04_merge_schedules/README.md) | Intervals: sort, then sweep | O(n log n) |
| 05 | [The Harvester Speed](challenges/05_min_harvest_speed/README.md) | Binary search on the answer | O(n log M) |

To find the right technique for a new problem, see [docs/patterns.md](docs/patterns.md), which groups the challenges by pattern and lists what each pattern "smells like".

## Requirements

- A C++20 compiler (Apple clang 15+, GCC 11+, MSVC 2022)
- CMake 3.24 or newer
- Git (GoogleTest is fetched automatically, unless it is already installed on your machine)

Developed with CMake 4.5 and Apple clang 21. On macOS you can speed up the first configure with `brew install googletest`, and CMake will use that copy instead of downloading one.

## Quick start

A longer guide that explains every command and option is in [docs/getting-started.md](docs/getting-started.md). To publish your own copy, see [docs/github-first-push.md](docs/github-first-push.md).

```bash
cmake --preset debug          # configure (downloads GoogleTest the first time)
cmake --build --preset debug  # build everything
ctest --preset debug          # run all tests
```

Presets available: `debug`, `release`, and `sanitize` (Debug with AddressSanitizer and UBSan, which catches memory bugs; CI uses it).

## Run one challenge

Every challenge `X` creates three targets: `X_lib` (the solution), `X_test` (the tests) and `X_trace` (the step-by-step demo). The challenge folder name is `NN_X`.

```bash
# Build only this challenge
cmake --build --preset debug --target maximal_square_test maximal_square_trace

# Run only its tests (each challenge's tests carry a label with its name)
ctest --preset debug -L maximal_square

# Or run the test binary directly, with a filter
./build/debug/challenges/01_maximal_square/maximal_square_test --gtest_filter='*Random*'

# Watch the algorithm step by step
./build/debug/challenges/01_maximal_square/maximal_square_trace
```

Other useful commands:

```bash
ctest --preset debug -R CountFields       # filter by test name
ctest --preset debug --output-on-failure  # show details when a test fails
cmake --preset release -DALGOLAB_BUILD_TRACES=OFF   # skip the trace demos
```

## How to study a challenge

1. Read the problem in the challenge's `README.md`. **Stop** before the solution and try it yourself, on paper first.
2. Compare your clarifying questions and brute force with the README's.
3. Run the `_trace` demo and follow it against the README's explanation.
4. Read the `.cpp` file. The comments explain why each line is the way it is, not only what it does.
5. Break it on purpose. Remove the `std::max` in challenge 04, or the `>= left` in challenge 03, and watch which test fails. The tests were written to catch exactly these mistakes.
6. Re-implement it from a blank file without looking.

## Repository layout

```
algorithms-lab/
├── CMakeLists.txt            # root build, language settings, warnings
├── CMakePresets.json         # debug / release / sanitize presets
├── cmake/
│   ├── AddChallenge.cmake    # add_challenge(): one line per challenge
│   └── GoogleTestSetup.cmake # find or fetch GoogleTest
├── docs/
│   ├── getting-started.md    # build, test, run: every command explained
│   ├── github-first-push.md  # git and GitHub: first push explained
│   └── patterns.md           # challenges grouped by technique
├── challenges/
│   ├── CMakeLists.txt        # auto-discovers folders named NN_*
│   └── 01_maximal_square/
│       ├── README.md                 # the full lesson
│       ├── CMakeLists.txt            # add_challenge(maximal_square)
│       ├── maximal_square.h          # the public function and its contract
│       ├── maximal_square.cpp        # the commented solution
│       ├── maximal_square_test.cpp   # GoogleTest tests
│       └── maximal_square_trace.cpp  # step-by-step terminal demo
├── templates/challenge/      # starting files for a new challenge
├── scripts/new_challenge.sh  # creates a new challenge from the template
├── LICENSE                   # MIT
├── AGENTS.md                 # instructions for AI assistants (any tool)
├── CLAUDE.md                 # Claude Code entry point (points to AGENTS.md)
└── .github/                  # CI workflow and Copilot instructions
```

## Adding a new challenge

```bash
./scripts/new_challenge.sh word_ladder "Word Ladder"
```

This picks the next number, creates `challenges/06_word_ladder/` from the template, and the build finds it automatically. Re-run `cmake --preset debug` once, then fill in the files. [AGENTS.md](AGENTS.md) describes the standard every challenge follows, which is also what an AI assistant is told to follow.

## Working with AI assistants

The repository is set up so Claude, Copilot, Codex or any other assistant follows the same conventions. [AGENTS.md](AGENTS.md) holds the rules. `CLAUDE.md` and `.github/copilot-instructions.md` simply point to it, so there is one source of truth.

## License

[MIT](LICENSE). You can use, copy, modify and share the code freely.

## Code style

C++20, no compiler extensions, warnings on (`-Wall -Wextra -Wpedantic -Wshadow`). Format with `clang-format` (the style is in `.clang-format`).
