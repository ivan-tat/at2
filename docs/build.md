# How to build the project

## Using GNU/Linux or compatible environment

### 1. Create `Build.sh` script in the project's root:

```bash
#!/bin/sh -e
. ./Build.conf
mkdir -p build
make $MAKEFLAGS -w -C build -f ../Makefile AT2_VERSION=1.0.0-release $*
```

and make it executable (run `chmod +x Build.sh`)

### 2. Create `Build.conf` file in the project's root:

```bash
#!/bin/sh -e
export PATH=$HOME/.local/opt/cross-fpc/bin:$PATH
export GCCFLAGS="-O2"
export FPCFLAGS="@$HOME/.local/opt/cross-fpc/fpc.cfg -O2"
MAKEFLAGS="-j $(nproc)"
```

You should prepare appropriate environment for cross-compilation in this case.

For custom optimizations you should provide appropriate options for GCC and FPC.

Example options for Pentium 2 CPU (setting these variables will prevent automatic selection of target CPU options in makefiles):

```bash
export GCCFLAGS_CPU="-march=pentium2 -m32"
export FPCFLAGS_CPU="-Pi386 -OpPENTIUM2"
```

For GCC use `gcc --help=target` and `gcc --help=optimizers` to check all supported options.

For FPC use `fpc -i` to check all supported options.

### 3. Build the project

```bash
./Build.sh
```

You may specify additional parameters for `Build.sh`. Example:

```bash
./Build.sh clean
```

See `Makefile.txt` for details.
