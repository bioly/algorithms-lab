# Getting started: build, test and run

This guide explains every command and option you need, not only what to type. It is also a small CMake and CTest lesson.

## The big picture

**CMake** doesn't compile anything itself. It reads the `CMakeLists.txt` files and *generates* a build system (on macOS usually Makefiles). That build system then calls the compiler. So there are three phases:

| Phase | Command | What happens |
|---|---|---|
| **Configure** | `cmake --preset debug` | Read the CMake files, find the compiler, download GoogleTest, write the build files into `build/debug/`. Nothing is compiled yet. |
| **Build** | `cmake --build --preset debug` | Run the generated build system, which compiles and links the programs. |
| **Test** | `ctest --preset debug` | Run the test programs and report pass or fail. |

Re-run **configure** only when CMake files change or when you add a challenge folder. Re-run **build** after every code change.

## 1. Get the code

**From a clone** (once the repo is on GitHub):

```bash
git clone git@github.com:YOUR_USER/algorithms-lab.git ~/dev/algorithms-lab
cd ~/dev/algorithms-lab
```

`git clone <url> <folder>` downloads the repository with its whole history into `<folder>`. Use the HTTPS URL if you don't use SSH keys.

**From a zip:**

```bash
cd ~/Downloads
unzip algorithms-lab-v0.3.zip        # extracts into a folder called algorithms-lab
mkdir -p ~/dev && mv algorithms-lab ~/dev/
cd ~/dev/algorithms-lab
```

`mkdir -p` creates the folder (and parents) if it doesn't exist, without complaining if it does. **Run every later command from this folder, the repository root.**

## 2. Check your tools

```bash
cmake --version      # 3.24 or newer
clang++ --version    # must support C++20 (Apple clang 15+)
git --version
```

On macOS, `xcode-select --install` installs the compiler and git.

Optional: `brew install googletest`. CMake looks for an installed GoogleTest first, so this skips the download and makes the first configure work offline.

## 3. Configure

```bash
cmake --preset debug
```

- `--preset debug` reads `CMakePresets.json` and uses the preset named `debug`. A preset is a saved set of options, so you don't retype them.
- The `debug` preset sets `binaryDir = build/debug` (where all generated files go, so the source folders stay clean and `build/` is git-ignored) and `CMAKE_BUILD_TYPE=Debug` (no optimization, debug symbols: `-O0 -g`).
- Without presets, the equivalent is `cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug`. `-S` is the source folder, `-B` is the build folder, and `-D NAME=VALUE` sets a cache variable.

The first run prints a lot, including the GoogleTest download. It should end with `Build files have been written to: .../build/debug`.

## 4. Build

```bash
cmake --build --preset debug -j
```

- `--build` means "run the build tool for me". It works the same with Makefiles, Ninja or Xcode.
- `--preset debug` picks the build folder from the preset. It is the same as `cmake --build build/debug`.
- `-j` compiles in parallel on all cores. A number limits it, for example `-j 4`.

| Option | Meaning |
|---|---|
| `--target maximal_square_test` | Build only that target and its dependencies. Faster when working on one challenge. |
| `--clean-first` | Delete old build results first, then build. |
| `-v` / `--verbose` | Print the full compiler commands. Good for learning which flags are used. |

## 5. Test

```bash
ctest --preset debug
```

`ctest` is CMake's test runner. It runs every test CMake knows about, and each GoogleTest case is one test. Expected end of output: `100% tests passed, 0 tests failed out of 50` (the number grows as challenges are added).

| Command | Meaning |
|---|---|
| `ctest --preset debug -L maximal_square` | `-L` filters by **label**. Every test is labeled with its challenge name, so this runs one challenge. |
| `ctest --preset debug -R CountFields` | `-R` filters by a **regular expression on the test name**. |
| `ctest --preset debug -E Random` | `-E` **excludes** tests whose names match. |
| `ctest --preset debug -N` | `-N` lists tests without running them. |
| `ctest --preset debug -V` | `-V` is verbose: shows the output of every test. |
| `ctest --preset debug --output-on-failure` | Prints a test's output only if it fails. (The presets already enable this.) |
| `ctest --preset debug --rerun-failed` | Runs only the tests that failed last time. |
| `ctest --preset debug -j 4` | Runs 4 tests in parallel. |

### Running a test program directly

Each `X_test` is a normal GoogleTest program with its own options:

```bash
./build/debug/challenges/01_maximal_square/maximal_square_test
./build/debug/challenges/01_maximal_square/maximal_square_test --gtest_list_tests
./build/debug/challenges/01_maximal_square/maximal_square_test --gtest_filter='*Random*'
./build/debug/challenges/01_maximal_square/maximal_square_test --gtest_repeat=5 --gtest_shuffle
```

- `--gtest_list_tests` shows the test names.
- `--gtest_filter='pattern'` runs the matching tests. `*` is a wildcard, and `-` excludes, for example `'*:-*Random*'`.
- `--gtest_repeat=N` runs the suite N times, and `--gtest_shuffle` randomizes the order. Together they expose tests that depend on each other.

## 6. Run a trace demo

```bash
./build/debug/challenges/01_maximal_square/maximal_square_trace
```

This prints the algorithm step by step. Every challenge has a `<name>_trace` program.

## 7. Presets and options

| Preset | What it does |
|---|---|
| `debug` | No optimization, debug symbols. Best for learning and debugging. |
| `release` | `-O3 -DNDEBUG`: optimized, as fast as it gets. Use it to see real performance. |
| `sanitize` | Debug plus `-fsanitize=address,undefined -fno-omit-frame-pointer`. CI uses it. |

What the sanitizer flags mean:

- **AddressSanitizer (`address`)** stops with a clear report on out-of-bounds access, use-after-free and similar memory bugs.
- **UndefinedBehaviorSanitizer (`undefined`)** catches things like signed integer overflow.
- `-fno-omit-frame-pointer` keeps stack traces in the reports readable.

Use any preset the same way, for example:

```bash
cmake --preset sanitize && cmake --build --preset sanitize -j && ctest --preset sanitize
```

Project options, set with `-D` when configuring:

| Option | Default | Meaning |
|---|---|---|
| `ALGOLAB_BUILD_TRACES` | `ON` | `OFF` skips building the trace demos. |
| `ALGOLAB_WARNINGS_AS_ERRORS` | `OFF` | `ON` makes any compiler warning fail the build (CI does this). |

Example: `cmake --preset debug -DALGOLAB_WARNINGS_AS_ERRORS=ON`. CMake **remembers** `-D` values in `build/debug/CMakeCache.txt`. To get back to the defaults, set the option explicitly, or delete the folder with `rm -rf build/debug` and configure again. Deleting `build/` is always safe, because it only holds generated files.

## 8. If something goes wrong

| Symptom | Likely cause | Fix |
|---|---|---|
| `error downloading googletest` | No internet, or a proxy or firewall blocks GitHub | `brew install googletest`, delete `build/`, configure again |
| `Unknown preset "debug"` | You are not in the repository root | `cd` to the folder that contains `CMakePresets.json` |
| `No tests were found!!!` | Not built yet, or wrong folder | Build first, then run `ctest` from the root |
| A new challenge folder isn't built | CMake hasn't re-scanned | Run `cmake --preset debug` again |
| C++20 compile errors | Old compiler | `xcode-select --install`, or update Xcode |
| Strange leftovers after big changes | Stale cache | `rm -rf build` and start again |
