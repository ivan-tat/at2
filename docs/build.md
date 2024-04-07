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
export FPCFLAGS=@$HOME/.local/opt/cross-fpc/fpc.cfg
MAKEFLAGS="-j $(nproc)"
```

You should prepare appropriate environment for cross-compilation in this case.

### 3. Build the project

```bash
./Build.sh
```

You may specify additional parameters for `Build.sh`. Example:

```bash
./Build.sh clean
```

See `Makefile.txt` for details.
