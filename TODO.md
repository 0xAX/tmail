# TODO

## General

  * Add info about build (openssl version and etc..) to help output
  * Use `strncmp(3)` instead of `strcmp(3)` when we know a length of a string.

## Build

  * Add header files as prerequisites in make files.
  * Strip libraries and executables if the `DEBUG` flag wasn't passed.
  * Disable `asserts` from `assert.h` for non-debug builds.
  * Add tests specific targets (like libtmail-sys.so and etc.) for `make test`.
  * Add common name for `%.o: %.c` targets.
  * Docker container.
  * Add stackprotector.
  * Build deb package.
  * Build rpm package.
  * Make `make install` and `make uninstall` more verbose.
  * Use sanitizers during build.
  * Support for [bsd make](https://www.freebsd.org/cgi/man.cgi?make(1)).
  * Move `LIBS` to `mk/Build.mk`

## tmail

  * use pledge(2) for OpenBSD builds.
  * Do not exit from tmail sub-commands, just go to the `main()` back.
  * Return from tmail's `main()` in a `*_cmd(argc, argv)`
  * Move compiler attributes to the `src/libutils/basic.h` as macros.
  * Support for colorful error messages.

## tmail daemon

  * Add initial implementation.

## send-email

  * More verbose error when attachment is not found.
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

  * Free memory in a case of parsing of configuration file error.
  * Rename `build_config_name()` to something like `get_config_path()`.
  * Remake `get_config_entry()`:

1. make it return `void*`.
2. add second parameter to append extension like `.smtprc` and etc.

  * Dump configuration in a pretty way in a case of `tmail config`
without configuration.
  * Add `smpt.encoding` configuration parameter.
  * Release everything in `at_exit()` callback.
  * check `state` during parsing of `smtp.signature`.
  * add `smtp.signature_path` instead of current `smtp.signature`.

## libsmtp

  * Add `smtp.default_subject` configuration item.
  * specify `MAX_ATTACHMENT_SIZE` during build.
  * Add send message in the separate thread.
  * Check message size if `SIZE` capability is supported.
  * Implement SMTP extensions.

## tmail smtp-caps

  * support for caps after starttls

## tmail smtp-server-info

  * Add support for help argument
(See https://tools.ietf.org/html/rfc5321#section-4.1.1.8)

## MIME

  * Make return value of the `get_mime_type()` - `const`.
  * Do not hard-code "contrib/mime.types" in `tmail.c`.
  * Add mime type that you like.

## libutils

### list_t

  * add implementation `list_insert(list_t*, void*, int position)`
  * add implementation `list_last(list_t *)`

### bitmap_t

  * Add initial implementation.

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
  * `smtp.signature` should be real signature.

## Testing

  * Add tests for `hostname()`.
  * Add tests for `fd_valid()`.
  * Add tests for `is_valid_ip_address()`.
