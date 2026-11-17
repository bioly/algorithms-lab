# AGENTS.md

Instructions for any AI coding assistant (Claude, Copilot, Codex, ...) working in this repository. Human contributors can follow them too.

## What this repository is

A learning spot for tricky algorithm problems in C++20. The audience is a developer preparing for technical interviews. **Teaching clarity matters more than cleverness.** Every challenge must be easy to follow for a junior developer.

## Commands

```bash
cmake --preset debug && cmake --build --preset debug && ctest --preset debug
ctest --preset debug -L <challenge_name>              # one challenge
cmake --preset sanitize && cmake --build --preset sanitize && ctest --preset sanitize
```

Always run the tests with the `sanitize` preset before saying work is finished.

## Repository conventions

- One folder per challenge: `challenges/NN_snake_case_name/`, where `NN` is the next free two-digit number. The build discovers folders matching `[0-9][0-9]_*` automatically. Never edit `challenges/CMakeLists.txt` to add one.
- The challenge's short name (`snake_case`, without the number) is used for all file names and targets.
- Use `scripts/new_challenge.sh <name> "<Title>"` to create the skeleton. Do not copy folders by hand.
- Code is in `namespace algolab`. C++20. No compiler extensions. No new third-party dependencies without asking.
- Tests use GoogleTest. Do not add another test framework.

## Files every challenge must have

| File | Purpose |
|---|---|
| `README.md` | The lesson. See "README standard" below. |
| `CMakeLists.txt` | One line: `add_challenge(<name>)` |
| `<name>.h` | The public function, with a comment stating the contract (inputs, outputs, preconditions, complexity) |
| `<name>.cpp` | The solution, with teaching comments |
| `<name>_test.cpp` | GoogleTest tests |
| `<name>_trace.cpp` | A standalone program that prints the algorithm's state step by step on one small example |

## Code standard

- **Comments explain why, not what.** Every non-obvious decision gets a comment: why this container, why this type (`int` vs `size_t`), why `<=` and not `<`, what bug the line prevents.
- Mention the trap or edge case a naive solution falls into, in the line that avoids it.
- Prefer the standard library. Prefer `const&` for read-only inputs, `std::string_view` for read-only text.
- Avoid recursion where the depth can be large. Say so in a comment when choosing an explicit stack.
- Mention overflow where it is possible, and use a wide enough type.
- Keep each solution to a single clear function, plus small private helpers in an anonymous namespace in the `.cpp`.
- Format with `clang-format`.

## Test standard

Every challenge's tests must include:

1. Empty or minimal input.
2. The worked example from the README.
3. The specific trap the challenge teaches (the case that a plausible but wrong solution fails). Name the test after it.
4. A large input, if performance or stack depth is part of the lesson.
5. A **randomized cross-check against a brute force** implementation, with a **fixed seed** so failures are reproducible.

Work out expected values by hand and double check them. A wrong expected value is a test bug, not a solution bug.

## Trace demo standard

- It is a separate `main()` that reproduces the algorithm with print statements. It does not need to share code with the solution (the solution should stay clean), but it must produce the same results.
- Output is plain ASCII, readable in an 80-column terminal, and shows how the state evolves at each step (cursor markers like `[x]`, `L`/`R`/`M`, brackets, small tables).
- It names the decision made at each step ("overlap -> merge", "GAP -> new block").

## README standard (the lesson)

Use this order. Look at `challenges/01_maximal_square/README.md` as the model.

1. Title, and a small table: pattern, difficulty, links to the files.
2. **The problem**, as a farmer story with a small example and answer.
3. **Clarifying questions** to ask the interviewer.
4. **Brute force**, and why it is too slow.
5. **The key idea**, in plain words, with a small drawing. Explain *why* it is correct.
6. **Watch it run**, with real output copied from the trace demo.
7. **Complexity** with a one-line reason each for time and space.
8. **Tests** table, including what each case checks.
9. **Interview talking points**, common bugs, follow-up questions.
10. **How to run**, with the exact commands for this challenge.

Write for a junior developer: short sentences, no unexplained jargon, the story first and the formal name second.

## Definition of done

- [ ] Builds without warnings (`-DALGOLAB_WARNINGS_AS_ERRORS=ON`).
- [ ] All tests pass under the `sanitize` preset.
- [ ] The trace demo runs, and the README's output matches it.
- [ ] The README's complexity claims are correct.
- [ ] The challenge is listed in the root `README.md` table and in `docs/patterns.md` under its primary pattern (add a new pattern section if needed).
- [ ] No build output or editor files are staged (see `.gitignore`).

## Do not

- Do not push, force-push, or rewrite git history unless the user asks.
- Do not add dependencies, change the C++ standard, or restructure folders without asking.
- Do not write tests that merely mirror the implementation. A test should check behavior that was worked out independently.
- Do not remove a failing test to make the suite pass.
