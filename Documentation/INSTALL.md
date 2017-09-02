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

*TODO*

After building is done, would be great to execute tests with:

```
$ make test
```
