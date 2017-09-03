# TODO

## General

  * Add `uchar_t` to `<basic.h>`.

## Build

  * Add ` -Weverything` for clang build.
  * Add enumeration of what avaliable targets will be built with the
`make all`.
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
  * Add `install_docs` target.

## tmail

  * Move compiler attributes to the `src/libutils/basic.h` as macros.
  * Support for colorful error messages.

## send-email

  * Catch `SIGINT` to release memory;
  * Add `--from-file` command line option.
  * If from file wasn't given and stdin is empty (select/poll), open editor.
  * Move to builtins directory.
  * Check that only one `from` is passed.
  * Support for message preview.

## libencoding

  * Add base64 `base64_encode()`.

## config

  * Remake `get_config_entry()`:

1. make it return `void*`;
2. add second parameter to append extension like `.smtprc` and etc.

  * Verbose errors in parser.
  * Trim values in the `fill_smtp_conf()`.
  * Rename `extern hashmap_t *conf;` to `conf_map` or something like this.
  * Add initial version and think about configuration syntax.
  * Dump configuration in a pretty way in a case of `tmail config`
without configuration.
  * Add `smpt.encoding` configuration parameter.
  * Release everything in `at_exit()` callback.

## libsmtp

  * specify `MAX_ATTACHMENT_SIZE` during build.
  * Add send message in the separate thread.
  * Add comments to describe SMTP capabilties in the smtp.h
  * Generate Message-Id header.
  * Check message size if `SIZE` capability is supported.
  * Send 'Reply-to:' if need (add a field to message struct).

## libmail

  * Add initial `libmail` template.
  * Add `mail` structure which should contain prepared email
to send.

## MIME

  * Make return value of the `get_mime_type()` - `const`.
  * Do not hard-code "contrib/mime.types" in `tmail.c`
  
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
  * Add `version.sh` to generate version.
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

  * Describe that `smtp.signature` is path to file with a signature.
  * Update `Documentation/INSTALL.md`:

1. Add information about make parameters
2. Add information about variables that can be overwritten (defined with `?=`).

  * Add `STANDARDS` section to man pages. (See OpenBSD).

## Testing

  * Add tests for `hostname()`.
  * Add tests for `fd_valid()`.
  * Add tests for `is_valid_ip_address()`.
  * Add tests for configuration parser.