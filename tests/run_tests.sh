#!/bin/bash
# Run from the project folder:  bash tests/run_tests.sh
cd "$(dirname "$0")/.." || exit 1
FLAGS="-std=c++11 -Wall -Wextra -g"
pass=0; fail=0
ok()  { echo "PASS: $1"; pass=$((pass+1)); }
bad() { echo "FAIL: $1"; fail=$((fail+1)); }

echo "--- unit tests ---"
( cd tests && g++ $FLAGS -o t_taha test_taha.cpp ../src/common.cpp ../src/filereader.cpp ../src/hashtable.cpp && ./t_taha | tail -1 )
( cd tests && g++ $FLAGS -o t_sha test_shahamat.cpp ../src/common.cpp ../src/rabinkarp.cpp ../src/maxheap.cpp && ./t_sha | tail -1 )
rm -f tests/t_taha tests/t_sha

echo "--- end-to-end ---"
g++ $FLAGS -o plagcheck src/*.cpp || exit 1
D=tests/data
run() { printf "$1" | ./plagcheck 2>&1; }

out=$(run "1\n$D/original.txt\n1\n$D/copy_exact.txt\n3\nn\n0\n")
echo "$out" | grep -q "100.00%" && ok "identical files -> 100%" || bad "identical files"
out=$(run "1\n$D/original.txt\n1\n$D/unrelated.txt\n3\nn\n0\n")
echo "$out" | grep -q " 0.00%" && ok "unrelated files -> 0%" || bad "unrelated files"
out=$(run "1\n$D/original.txt\n1\n$D/reordered.txt\n3\nn\n0\n")
echo "$out" | grep -q "58.02%" && ok "reordered copy -> 58.02%" || bad "reordered copy"
out=$(run "1\n$D/missing.txt\n0\n")
echo "$out" | grep -q "file not found" && ok "missing file handled" || bad "missing file"
out=$(run "1\n$D/empty.txt\n0\n")
echo "$out" | grep -q "is empty" && ok "empty file handled" || bad "empty file"
out=$(run "1\n$D/one_word.txt\n1\n$D/short.txt\n3\nn\n0\n")
echo "$out" | grep -q " 0.00%" && ok "one word / tiny files -> 0%" || bad "tiny files"
out=$(run "1\n$D/original.txt\n3\n0\n")
echo "$out" | grep -q "at least 2 files" && ok "only 1 file handled" || bad "only 1 file"
out=$(run "1\n$D/punct_case.txt\n1\n$D/original.txt\n3\nn\n0\n")
echo "$out" | grep -q "100.00%" && ok "case and punctuation ignored" || bad "case/punctuation"
out=$(run "1\n$D/large_a.txt\n1\n$D/large_b.txt\n3\nn\n0\n")
echo "$out" | grep -q "9996 common" && ok "large input (30000 words) -> 9996 common chunks" || bad "large input"
out=$(run "abc\n9\n0\n")
echo "$out" | grep -q "Please type a number" && ok "bad menu input re-asked" || bad "bad menu input"
rm -f plagcheck
echo "Passed: $pass  Failed: $fail"
