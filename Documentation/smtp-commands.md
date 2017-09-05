# smtp-commands

`smtp-commands` command provides ability to get and display commands supported by an SMTP server.

```
$ tmail smtp-commands
Usage: tmail smtp-commands [--version] [--help] smtp_server port 
```

## Example of usage

To get commands supported by the [GMail](https://www.google.com/gmail/) SMTP server:

```
$ tmail smtp-commands smtp.gmail.com 587
```