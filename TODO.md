# TODO

## Build

  * Add enumeration of what avaliable targets will be built with the
`make all`.
  * Add description about `mk` build scripts for other `OSes`.
  * Update core `Makefile` for at least BSD `OSes`.
  * Strip libraries and executables if the `DEBUG` flag wasn't passed.
  * Fix -fmax-erros for osx gcc build.
  * Add `install` target.

## Platforms related

  * `OpenBSD`: use `snprintf` instead of `sprintf` because of a warn.
Maybe it would be good to replace it for all platforms for safety.

## Scripts

### check_coding_style

  * Check 80 columns in code, commit message, build files and docs.
  * Check that we have no anything before square brackets.
  * Check Sign-Off-By signature in a commit message.
  * Check header and empty line after it in a commit message.
  * Check that there are no one line comments.