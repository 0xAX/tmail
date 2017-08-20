# TODO

## General

  * Add `uchar_t` to `<basic.h>`.
  * Be more verbose in `fprintf`. For example pass a filename insterted
of just `No such file or directory`.

## Build

  * `make clean` does not work in `libencoding`. Maybe in other libs.
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

  * Move `options` outside of `parse_argv()`.
  * Move compiler attributes to the `src/libutils/basic.h` as macros.
  * Support for colorful error messages.
  * Return non-zero error code if we exit during failure case.
  * Move openning of a `mime.types` into tmail. To open it only once.

## send-email

  * Catch `SIGINT` to release memory;
  * Add `--from-file` command line option.
  * If from file wasn't given and stdin is empty (select/poll), open editor.
  * Move to builtins directory.
  * Check that only one `from` is passed.
  * Support for message preview.
  * Documentation!

## libencoding

  * Add base64 `base64_encode()`.

## config

  * Add initial version and think about configuration syntax.
  * Dump configuration in a pretty way in a case of `tmail config`
without configuration.

## libsmtp

  * Fix error message in `send_message_header()`.
  * specify `MAX_ATTACHMENT_SIZE` during build.
  * Specify blind copies in RCPT TO.
  * Add send message in the separate thread.
  * Get correct argument for `EHLO` command.
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

## libutils

### list_t

  * Replace it with `slist` from `libands`.

### stack_t

  * Remove it as we don't use it.

### bitmap_t

  * Add initial implementation.

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

  * Update `Documentation/INSTALL.md`:

1. Add information about make parameters
2. Add information about variables that can be overwritten (defined with `?=`).

  * Add `STANDARDS` section to man pages. (See OpenBSD).

## Testing

  * Add tests for `hostname()`.
  * Add tests for `fd_valid()`.
  * Add tests for `is_valid_ip_address()`.
