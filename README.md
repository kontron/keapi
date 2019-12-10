# Kontron Embedded API

A software library that enables programmers to easily create their applications
for monitoring and control hardware resources of Kontron boards, modules,
systems and platforms.

## Getting Started

These instructions will get you a copy of the project up and running on your
local machine for development and testing purposes.

### Prerequisites

Development tools:

- GNU Autotools:
  - [Autoconf](https://www.gnu.org/software/autoconf/)
  - [Automake](https://www.gnu.org/software/automake/)
  - [Libtool](https://www.gnu.org/software/libtool/)
- [GNU Make](https://www.gnu.org/software/make/)
- [GCC](https://gcc.gnu.org/)
- Cross-Development Toolchain(s) (*optional*)

Libraries:

- POSIX threads (**libpthread**)
- Perl Compatible Regular Expression library (**libpcre**)
- ATA S.M.A.R.T. reading and parsing library (**libatasmart**)
- [Jansson](https://github.com/akheron/jansson) (**libjansson**)

### Build instruction

**Compilation on target:**

```bash
# Configure with autoconf
autoreconf --install
./configure --prefix=/usr ${CONFIGURE_FLAGS}

# Build and install
make
make install
```

**Cross-compilation (example for ARM):**

```bash
# Setup environment
source /opt/poky/<version>/environment-setup-armv7vehf-neon-poky-linux-gnueabi

# Configure with autoconf
autoreconf --install
./configure --build=x86_64-linux-gnu \
            --host=arm-poky-linux-gnueabi \
            --prefix=/usr ${CONFIGURE_FLAGS}

# Build
make

# Copy compiled libraries to target file system
# scp .libs/<libname.[a,la,so]> root@<target_ip>:/usr/lib/
```

### Coding style

Modified .clang-format file from [Linux kernel project](https://www.kernel.org/)
with increased ColumnLimit (80 -> 120).

```bash
clang-format -style=file -i -verbose src/* include/*
```

## Configuration files

All configuration files are placed in the "/etc/keapi" directory of target file system.

See [CONFIGURATION_FILES_FORMAT](docs/CONFIGURATION_FILES_FORMAT) for details.

## Versioning

KEAPI version is four numbers divided by point (for example 3.0.5.0).

It contains two parts: `(a.b).(c.d)`.

First part - `a.b`, defined in keapi.h and reflects library specification:

- `a` - specifies basic API specification version
- `b` - reflects changes which are not related to basic API

Second part - `c.d`, is platform dependent:

- `c` - reflects functions behavior changes. Could affect on linked application
- `d` - reflects implementation fixes / improvements. Should not affect on linked application

## License

This project is licensed under the BSD 3-Clause "New" or "Revised" License - see the [LICENSE](LICENSE) file for details.
