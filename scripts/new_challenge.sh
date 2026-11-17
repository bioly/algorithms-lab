#!/usr/bin/env bash
# Creates a new challenge folder from templates/challenge.
#
#   ./scripts/new_challenge.sh <snake_case_name> "<Human Title>" [FunctionName]
#
# Example:
#   ./scripts/new_challenge.sh word_ladder "Word Ladder" ShortestLadder
#
# Works with the bash 3.2 and BSD sed that ship with macOS.
set -euo pipefail

if [ $# -lt 2 ]; then
    echo "usage: $0 <snake_case_name> \"<Human Title>\" [FunctionName]" >&2
    exit 1
fi

name="$1"
title="$2"

if ! printf '%s' "$name" | grep -Eq '^[a-z][a-z0-9_]*$'; then
    echo "error: name must be snake_case (a-z, 0-9, _), got '$name'" >&2
    exit 1
fi

# Default function name: snake_case -> CamelCase (word_ladder -> WordLadder)
if [ $# -ge 3 ]; then
    func="$3"
else
    func=""
    IFS='_' read -r -a parts <<< "$name"
    for p in "${parts[@]}"; do
        first="$(printf '%s' "${p:0:1}" | tr '[:lower:]' '[:upper:]')"
        func="${func}${first}${p:1}"
    done
fi
suite="$func"

root="$(cd "$(dirname "$0")/.." && pwd)"
tmpl="$root/templates/challenge"
challenges="$root/challenges"

# Refuse duplicates of the short name.
if ls "$challenges" | grep -Eq "^[0-9][0-9]_${name}\$"; then
    echo "error: a challenge named '$name' already exists" >&2
    exit 1
fi

# Next number = highest existing NN + 1.
last="$(ls "$challenges" | grep -E '^[0-9][0-9]_' | sed -E 's/^([0-9][0-9])_.*/\1/' | sort | tail -n 1)"
if [ -z "$last" ]; then last=0; fi
next=$(( 10#$last + 1 ))
nn="$(printf '%02d' "$next")"
dir="${nn}_${name}"
dest="$challenges/$dir"

mkdir -p "$dest"

render() { # render <template> <output>
    sed -e "s|__NAME__|$name|g" \
        -e "s|__TITLE__|$title|g" \
        -e "s|__NN__|$nn|g" \
        -e "s|__DIR__|$dir|g" \
        -e "s|__FUNC__|$func|g" \
        -e "s|__SUITE__|$suite|g" \
        "$1" > "$2"
}

render "$tmpl/CMakeLists.txt.in"   "$dest/CMakeLists.txt"
render "$tmpl/NAME.h.in"           "$dest/$name.h"
render "$tmpl/NAME.cpp.in"         "$dest/$name.cpp"
render "$tmpl/NAME_test.cpp.in"    "$dest/${name}_test.cpp"
render "$tmpl/NAME_trace.cpp.in"   "$dest/${name}_trace.cpp"
render "$tmpl/README.md.in"        "$dest/README.md"

echo "Created challenges/$dir"
echo "Next steps:"
echo "  1. cmake --preset debug          # re-run so the new folder is discovered"
echo "  2. fill in the TODOs in $dir/ (see AGENTS.md for the standard)"
echo "  3. add the challenge to the table in README.md and to docs/patterns.md"
