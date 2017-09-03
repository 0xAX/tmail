# send-email

`send-email` command provides ability to send email.

## Command line options

```
Usage:
 tmail send-email [options...]

Sender/Receiver options:
  -f, --from=<address>     specify author of an email
  -n, --realname=<name>    specify real name of an email author
  -s, --subject=<subj>     specify subject of an email
  -t, --to=<address>       specify the primary recipient of an
                           email. Multiply options are allowed
  -c, --cc=<address>       specify the secondary recipient of an
                           email. Multiply options are allowed
  -b, --bcc=<address>      specify the blind copy of an email
                           Multiply options are allowed
  -a, --attachment=<file>  add attachment to an email
                           Multiply options are allowed

Composing email mode:
  -e, --use-editor     use editor to compose a message
  -i, --interactive        compose an email in interactive mode
```

## Example of usage

Send an email with body from `MESSAGE_BODY` file:

```
$ ./tmail send-email -f username@mail.com         \
                     -t username2@mail.com        \
                     -c username3@mail.com        \
                     -s "Subject"                 \
                     -a /home/user/attachment.txt \
                     <MESSAGE_BODY
```

Send an email with body from `stdin`:

```
$ echo "Message body" | ./tmail send-email -f username@mail.com         \
                                           -t username2@mail.com        \
                                           -c username3@mail.com        \
                                           -s "Subject"                 \
                                           -a /home/user/attachment.txt
```
