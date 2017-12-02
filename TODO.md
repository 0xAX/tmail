# TODO

## General

  * Describe dependencies in README.md
  * Decrease number of header files in libraries, like in libsmtp.
  * Use `strncmp(3)` instead of `strcmp(3)` when we know a length of a string.

## Build

  * Fix build in CI
  * Add header files as prerequisites in make files.
  * Strip libraries and executables if the `DEBUG` flag wasn't passed.
  * Disable `asserts` from `assert.h` for non-debug builds.
  * Add tests specific targets (like libtmail-sys.so and etc.) for `make test`.
  * Add common name for `%.o: %.c` targets.
  * Docker container.
  * Add stackprotector.
  * Build deb package.
  * Build rpm package.
  * Add new macro to expand to source code dirs, like
`$(SRC_DIR)/$(TMAIL_SMTP_LIB_DIR)` -> `$(TMAIL_SMPT_SRC_DIR)` or something
like this.
  * Make `make install` and `make uninstall` more verbose.
  * Use sanitizers during build.
  * Support for [bsd make](https://www.freebsd.org/cgi/man.cgi?make(1)).

## tmail

  * Return from tmail's `main()` in a `*_cmd(argc, argv)`
  * Move compiler attributes to the `src/libutils/basic.h` as macros.
  * Support for colorful error messages.

## tmail daemon

  * Add initial implementation.

## send-email

  * Implement `-i/--interactive`.
  * Implement `-e/--use-editor`.
  * Add `--smtp-server`, `--smtp-server-address` and `--smtp-server-port`.
  * Add `--smtp-password`.
  * Catch `SIGINT` to release memory.
  * Add `--from-file` command line option.
  * If from file wasn't given and stdin is empty (select/poll), open editor.
  * Move to builtins directory.
  * Check that only one `from` is passed.
  * Support for message preview.

This stuff should be implemented after `STARTTLS` support:

  * Add `tls` related options.
  * Add `--no-starttls`.
 
## libencoding

  * Add base64 `base64_encode()`.

## config

  * Remake `get_config_entry()`:

1. make it return `void*`.
2. add second parameter to append extension like `.smtprc` and etc.

  * Dump configuration in a pretty way in a case of `tmail config`
without configuration.
  * Add `smpt.encoding` configuration parameter.
  * Release everything in `at_exit()` callback.

## libsmtp

  * specify `MAX_ATTACHMENT_SIZE` during build.
  * Add send message in the separate thread.
  * Check message size if `SIZE` capability is supported.
  * Implement SMTP extensions.

## smtp-server-info

  * Add support for help argument
(See https://tools.ietf.org/html/rfc5321#section-4.1.1.8)

## MIME

  * Make return value of the `get_mime_type()` - `const`.
  * Do not hard-code "contrib/mime.types" in `tmail.c`.
  * Add mime type that you like.

## libutils

### list_t

  * Replace it with `slist` from `libands`.

### stack_t

  * Remove it as we don't use it.

### bitmap_t

  * Add initial implementation.

### hashmap_t

  * Provide documentation.

## Scripts

### apply-clang-format.sh

  * Add `-f` option to pass a file to the script.
  * If there are no given parameters, try to execute script on the
current dir.

## libsys

  * add tests for is_valid_ip_address()
  * add IPv6 support to is_valid_ip_address()
  * add IPv6 support to connect_to_ip()

## Scripts (misc)

  * Add valgrind script.
  * Add bash completion.
  * Add scripts to do auto-commit of:

`git commit -S -s -v -m "general: apply-clang-format"`

and

`git commit -S -s -v -m "general: update TODO.md"`

## contrib

  * Add systemd file to run tmail as a daemon.
  * Add configuration examples to `contrib/` dir.

## Distribution

  * debian packge
  * arch linux package
  * fedora/centos package
  * .*bsd packages

## Documentation

  * Document 
  * Provide information about supported SMTP extensions to man and md pages.
  * Describe that `smtp.signature` is path to file with a signature.

## Testing

  * Add tests for `hostname()`.
  * Add tests for `fd_valid()`.
  * Add tests for `is_valid_ip_address()`.
  * Add tests for configuration parser.
