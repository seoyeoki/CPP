#!/bin/bash
# ex04 tests: run from anywhere, e.g. `bash tests/test.sh` or `make test`

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR/.." || exit 1      # run from ex04/ so the usage line shows "./replace"
BIN="./replace"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP" "$TMP.replace"' EXIT

PASS=0
FAIL=0

if [ ! -x "$BIN" ]; then
	echo "replace binary not found. Run 'make' first."
	exit 1
fi

ok()   { PASS=$((PASS + 1)); printf "  [ OK ] %s\n" "$1"; }
fail() { FAIL=$((FAIL + 1)); printf "  [FAIL] %s\n" "$1"; }

# run_case <name> <s1> <s2>
#   copies tests/input/<name>.txt to a temp dir, runs ./replace, and compares
#   <file>.replace with tests/expected/<name>.txt (byte for byte)
run_case()
{
	local name="$1" s1="$2" s2="$3"

	cp "$DIR/input/$name.txt" "$TMP/$name.txt"
	"$BIN" "$TMP/$name.txt" "$s1" "$s2" > /dev/null
	local rc=$?

	if [ $rc -ne 0 ]; then
		fail "$name (exit code $rc)"
	elif cmp -s "$TMP/$name.txt.replace" "$DIR/expected/$name.txt"; then
		ok "$name"
	else
		fail "$name (output differs)"
		diff <(od -c "$DIR/expected/$name.txt") <(od -c "$TMP/$name.txt.replace") | head -10
	fi

	# the original file must never be modified
	if ! cmp -s "$TMP/$name.txt" "$DIR/input/$name.txt"; then
		fail "$name (original file was modified)"
	fi
}

# expect_error <description> <expected message> <path that must NOT exist afterwards, or "-"> <args...>
#   the program must exit with code 1, print exactly <expected message>,
#   and must not leave an output file behind
expect_error()
{
	local desc="$1" expected_msg="$2" must_not_exist="$3"
	shift 3

	local actual_msg
	actual_msg="$("$BIN" "$@" 2>&1)"
	local rc=$?
	local problems=""

	[ $rc -eq 1 ]                        || problems="$problems\n      exit code : expected 1, got $rc"
	[ "$actual_msg" = "$expected_msg" ]  || problems="$problems\n      message   : expected [$expected_msg]\n                  actual   [$actual_msg]"
	if [ "$must_not_exist" != "-" ] && [ -e "$must_not_exist" ]; then
		problems="$problems\n      output    : $must_not_exist should not exist"
	fi

	if [ -z "$problems" ]; then
		ok "$desc -> exit 1, \"$expected_msg\""
	else
		fail "$desc"
		printf "$problems\n"
	fi
}

echo "== replacement cases =="
run_case basic            hello  bye
run_case no_trailing_nl   a      x
run_case s2_contains_s1   a      aa
run_case overlap_even     aa     b
run_case overlap_odd      aa     b
run_case no_match         zzz    Y
run_case delete_s1        -b     ""
run_case empty_file       a      b
run_case blank_lines      a      X
run_case at_both_ends     x      Y
run_case s1_longer        abcdef Z
run_case whole_content    hello  bye
run_case spaces           "hello world" hi
run_case multiline_s1     $'a\nb' X
run_case special_chars    "[d]"  "<d>"
run_case same_s1_s2       abc    abc
run_case crlf             b      X
run_case utf8_korean      안녕   bye
run_case nul_bytes        a      X
run_case high_bytes       a      Z

echo "== file handling =="
# filename containing a space
cp "$DIR/input/basic.txt" "$TMP/my file.txt"
"$BIN" "$TMP/my file.txt" hello bye > /dev/null
if cmp -s "$TMP/my file.txt.replace" "$DIR/expected/basic.txt"; then ok "filename with a space"; else fail "filename with a space"; fi

# an existing output file is overwritten, not appended to
cp "$DIR/input/basic.txt" "$TMP/ow.txt"
printf 'OLD OLD OLD OLD OLD OLD OLD OLD OLD OLD\n' > "$TMP/ow.txt.replace"
"$BIN" "$TMP/ow.txt" hello bye > /dev/null
if cmp -s "$TMP/ow.txt.replace" "$DIR/expected/basic.txt"; then ok "existing output file is overwritten"; else fail "existing output file is overwritten"; fi

echo "== error cases =="
USAGE="Usage: $BIN <filename> <s1> <s2>"
cp "$DIR/input/basic.txt" "$TMP/err.txt"
expect_error "no arguments"                "$USAGE" -
expect_error "too few arguments (1)"       "$USAGE" "$TMP/err.txt.replace"     "$TMP/err.txt"
expect_error "too few arguments (2)"       "$USAGE" "$TMP/err.txt.replace"     "$TMP/err.txt" hello
expect_error "too many arguments"          "$USAGE" "$TMP/err.txt.replace"     "$TMP/err.txt" hello bye extra
expect_error "file does not exist"         "Cannot open input file" "$TMP/nope.txt.replace" "$TMP/nope.txt" hello bye
expect_error "empty s1"                    "Invalid input"          "$TMP/err.txt.replace"  "$TMP/err.txt" "" bye
expect_error "empty filename"              "Invalid input"          ".replace"              "" hello bye
expect_error "input is a directory"        "Cannot read input file" "$TMP.replace"          "$TMP" hello bye

if [ "$(id -u)" -ne 0 ]; then
	cp "$DIR/input/basic.txt" "$TMP/noread.txt"
	chmod 000 "$TMP/noread.txt"
	expect_error "input file not readable" "Cannot open input file" "$TMP/noread.txt.replace" "$TMP/noread.txt" hello bye
	chmod 644 "$TMP/noread.txt"

	mkdir "$TMP/ro"
	cp "$DIR/input/basic.txt" "$TMP/ro/in.txt"
	chmod 555 "$TMP/ro"
	expect_error "output file cannot be created" "Cannot create output file" "$TMP/ro/in.txt.replace" "$TMP/ro/in.txt" hello bye
	chmod 755 "$TMP/ro"
fi

echo
echo "passed: $PASS, failed: $FAIL"
[ $FAIL -eq 0 ]
