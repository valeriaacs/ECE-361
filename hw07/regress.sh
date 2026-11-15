#!/bin/sh
set -u

failed=0
total=0

for infile in inputs/*.txt; do
    name=$(basename "$infile" .txt)
    total=$((total + 1))

    tmp_out=$(mktemp)
    ./balance < "$infile" > "$tmp_out"
    actual_rc=$?

    expected_out="expected/${name}.out"
    expected_rc=$(cat "expected/${name}.rc")

    if ! diff -u "$expected_out" "$tmp_out" > /dev/null; then
        echo "FAIL [$name]: output mismatch"
        diff -u "$expected_out" "$tmp_out"
        failed=$((failed + 1))
    elif [ "$actual_rc" -ne "$expected_rc" ]; then
        echo "FAIL [$name]: expected exit status $expected_rc, got $actual_rc"
        failed=$((failed + 1))
    else
        echo "PASS [$name]"
    fi

    rm -f "$tmp_out"
done

echo "Regression summary: $((total - failed))/$total passed"
[ "$failed" -eq 0 ]