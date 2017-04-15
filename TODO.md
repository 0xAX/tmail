# TODO

## Build

  * Add enumeration of what avaliable targets will be built with the
`make all`.
  * Add description about `mk` build scripts for other `OSes`.
  * Update core `Makefile` for at least BSD `OSes`.
  * strip libraries and executables if the `DEBUG` flag wasn't passed.
  * fix -fmax-erros for osx gcc build.
  * add `install` target.

## Misc

  * Add CI badge.

## Platforms related

  * `OpenBSD`: use `snprintf` instead of `sprintf` because of a warn.
Maybe it would be good to replace it for all platforms for safety.
