# TODO

## Build

  * Add ` -Weverything` for clang build.
  * Add enumeration of what avaliable targets will be built with the
`make all`.
  * Strip libraries and executables if the `DEBUG` flag wasn't passed.
  * Fix -fmax-erros for osx gcc build.
  * Disable `asserts` from `assert.h` for non-debug builds.
  * Add tests specific targets (like libtmail-sys.so and etc.) for `make test`.
  * Add common name for `%.o: %.c` targets.
  * Docker container.

## tmail

  * Add `-b` or `--bcc` for blind copy.
  * Add `isatty()` check to prevent raw-mode enabling from scripts
for example.

## libsmtp

  * Add send message in the separate thread.

## libsys

  * Move sys to libsys.

## libmail

  * Add initial `libmail` template.
  * Add `mail` structure which should contain prepared email
to send.

## libutils

### list_t

  * Implement `list_insert_after()`.
  * Implement `list_insert_before()`.
  * Add `test` + valgrind tests.
  * Describe `list_t` usage in a header of the source code file.

## Scripts

### check_coding_style

  * Check 80 columns in code, commit message, build files and docs.
  * Check that we have no anything before square brackets.
  * Check Sign-Off-By signature in a commit message.
  * Check header and empty line after it in a commit message.
  * Check that there are no one line comments.

### apply-clang-format.sh

  * Add `-f` option to pass file to the script.

## Scripts (misc)

  * Add valgrind script.
  * Add `version.sh` to generate version.
  * Add bash completion.