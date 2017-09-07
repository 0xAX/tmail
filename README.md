tmail
========

[![Build Status](https://travis-ci.org/0xAX/tmail.svg?branch=master)](https://travis-ci.org/0xAX/tmail) [![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
 
`tmail` is a set of text-based [email](https://en.wikipedia.org/wiki/Email) utils.

## Build and installation

See [INSTALL.md](https://github.com/0xAX/tmail/blob/master/Documentation/INSTALL.md).

## Usage

Main executable is `tmail`:

```
$ ./tmail 
usage: tmail [--version] [--help]

tmail provides following commands:

    * send-email - send an email
    * smtp-caps - show SMTP server capabilities
    * smtp-server-info - show userful information for the given SMTP server
    * config - manage tmail configuration

Each command has own set of command line arguments.
Use: tmail command -h/--help to get help
```

provides set of builtin commands like `send-email`, `config` and etc.
Each sub-command has own help. Additionally you may find more info at:

  * [send-email](https://github.com/0xAX/tmail/blob/master/Documentation/send-email.md)

or in the man pages.

## Support for protocols

### SMTP

  * [RFC 5321](https://github.com/0xAX/tmail/blob/master/rfc/rfc-5321.txt) - Simple Mail Transfer Protocol. Support for base commands `EHLO`, `MAIL FROM`, `RCPT TO`, and etc...

Following SMTP extensions are supported:

  * [HELP](https://tools.ietf.org/html/rfc5321#section-4.1.1.8)

### MIME

  * [RFC 4648](https://github.com/0xAX/tmail/blob/master/rfc/rfc-4648.txt) - The Base16, Base32, and Base64 Data Encodings. To send attachments in base64 `Content-Transfer-Encoding`.

### IMAP

### POP3

## Contributing

Feel free to send a [pull request](https://help.github.com/articles/about-pull-requests/) if you found something wrong or you have any ideas.

If you don't know where to start, see [TODO.md](https://github.com/0xAX/tmail/blob/master/TODO.md).

If you have any questions or suggestions, feel free to create new [issue](https://github.com/0xAX/tmail/issues/new) or drop me an [email](mailto:kuleshovmail@gmail.com).

## Author

[@0xAX](https://twitter.com/0xAX)