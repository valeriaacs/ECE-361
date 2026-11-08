#!/bin/bash
set -e

echo "Running regression tests..."

SCENARIOS=("scenario1.txt" "scenario2.txt")
FAIL=0

for sc in "${SCENARIOS[@]}"; do
    echo -n "Checking $sc... "
    ./replay "scenarios/$sc" > "tmp_$sc.trace"
    
    if diff -u "expected/$sc.trace" "tmp_$sc.trace" > /dev/null; then
        echo "PASS"
        rm -f "tmp_$sc.trace"
    else
        echo "FAIL"
        diff -u "expected/$sc.trace" "tmp_$sc.trace"
        rm -f "tmp_$sc.trace"
        FAIL=1
    fi
done

if [ $FAIL -ne 0 ]; then
    echo "Regression tests failed."
    exit 1
fi

echo "All regression tests passed!"
exit 0