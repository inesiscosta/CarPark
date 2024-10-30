# Directory containing the test files
TEST_DIR="private-tests"

EXE="./project"

LOG_FILE="valgrind.log"

> "$LOG_FILE"

for test_file in "$TEST_DIR"/*.in; do
   valgrind "$EXE" < "$test_file" > /dev/null 2>> "$LOG_FILE"
done
