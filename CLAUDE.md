# CLAUDE.md

Read [AGENTS.md](AGENTS.md) first. It holds the repository conventions, the standards for code, tests, trace demos and READMEs, and the definition of done. It applies to you in full.

Notes specific to Claude Code:

- The user is learning. When asked for a new challenge, follow the routine used so far: clarifying questions, brute force, key insight, trace, code, complexity, tests. Build and run the code before presenting results, and copy the trace demo's real output into the README rather than writing it by hand.
- Create new challenges with `scripts/new_challenge.sh`.
- Run `cmake --preset sanitize && cmake --build --preset sanitize && ctest --preset sanitize` before reporting that work is done.
- Do not commit or push unless asked. If asked to commit, group the change per challenge.
