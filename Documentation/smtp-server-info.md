# smtp-server-info

`smtp-server-info` command provides ability to get and display commands supported by an SMTP server
and addinitial useful information about an SMTP server.

```
$ tmail smtp-server-info
Usage: tmail smtp-server-info [--version] [--help] smtp_server port 
```

## Example of usage

```
$ tmail smtp-server-info 127.0.0. 25

SMTP Server information:

214-Commands supported:
214 AUTH HELO EHLO MAIL RCPT DATA BDAT NOOP QUIT RSET HELP
```