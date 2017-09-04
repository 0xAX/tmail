# Build and install

To build `tmail` from source code, execute following instructions:

1. Clone repo:

```
$ git clone git@github.com:0xAX/tmail.git
```

2. Go to clonned repo:

```
$ cd tmail
```

3. Execute make:

```
$ make
```

or:

```
$ gmake
```

on `*BSD` systems.

## Additional dependencies

You may need to install following dependencies:

  * [bison](https://www.gnu.org/software/bison/)
  * [flix](https://www.gnu.org/software/flex/)
  * [make](https://www.gnu.org/software/make/)

And of course compilers

  * [gcc](https://gcc.gnu.org/) - minimal version is - `4.8`
  * [clang](https://clang.llvm.org/) minimal version is `4.0`

## Build customization

Additionally following flags can be passed to make:

  * `TMAIL_CC` to override default compiler (by default `gcc` is used)
  * `V=1` for a more verbose build
  * `DEBUG=1` to build tmail with debug info.
  * `NON_NATIVE_BINARY` to disable -march=native
  * `LIBDIR` to specify where to install tmail shared libraries
  * `BINDIR` to specify where to install tmail executables
  * `MANDIR` to specify where to install tmail man pages

For example:

```
$ sudo make install MANDIR=/usr/share
```

or

```
$ make DEBUG=1
```

After building is done, would be great to execute tests with:

```
$ make test
```
