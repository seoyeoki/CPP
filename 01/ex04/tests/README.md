# ex04 tests

```
make              # build ./replace first
bash tests/test.sh
```

- `input/<name>.txt`: input file for each case
- `expected/<name>.txt`: exact expected content of `<name>.txt.replace` (compared byte for byte with `cmp`)
- `test.sh`: runs every case on a temporary copy (the originals in `input/` are never touched), then checks the error cases (wrong argument count, missing file, empty `s1`, empty filename, directory as input, unreadable input, unwritable output). Each error case checks **three things**: exit code is 1, the printed message is exactly the expected one, and no output file was left behind. On failure it prints expected vs actual.

The script exits with a non-zero code if any case fails.
