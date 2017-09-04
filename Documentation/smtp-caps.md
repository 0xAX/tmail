# smtp-caps

`smtp-caps` command provides ability to get and display capabilities of an SMTP server.

## Command line options

```
Usage: tmail smtp-caps [--version] [--help] smtp_server port
```

## Example of usage

To get capabilities of [GMail](https://www.google.com/gmail/) SMTP server:

```
$ tmail smtp-caps smtp.gmail.com 587
SIZE
8BITMIME
PIPELINING
CHUNKING
STARTTLS
ENHANCEDSTATUSCODES
SMTPUTF8
```