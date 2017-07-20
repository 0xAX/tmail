tmail
========

[![Build Status](https://travis-ci.org/0xAX/tmail.svg?branch=master)](https://travis-ci.org/0xAX/tmail) [![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
 
`tmail` is a set of text-based [email](https://en.wikipedia.org/wiki/Email) utils.

## Build and installation

Clone repo:

```
$ git clone git@github.com:0xAX/tmail.git
```

Go to clonned repo:

```
$ cd tmail
```

and execute make:

```
$ make
```

or:

```
$ gmake
```

on `*BSD` systems.

Additionally following flags can be passed to make:

*TODO*

After building is done, would be great to execute tests with:

```
$ make test
```

## Usage

Main executable is `tmail`:

```
$ ./tmail 
usage: tmail [--version] [--help]

tmail provides following commands:

    * send-email - send an email
    * config - manage tmail configuration

Each command has own set of command line arguments.
Use: tmail command -h/--help to get help
```

provides set of builtin commands like `send-email`, `config` and etc.
Each sub-command has own help.

## Contributing

Feel free to send a [pull request](https://help.github.com/articles/about-pull-requests/) if you found something wrong or you have any ideas.

If you don't know where to start, see [TODO.md](https://github.com/0xAX/tmail/blob/master/TODO.md).

If you have any questions or suggestions, feel free to create new [issue](https://github.com/0xAX/tmail/issues/new) or drop me an [email](mailto:kuleshovmail@gmail.com).

## Author

[@0xAX](https://twitter.com/0xAX)