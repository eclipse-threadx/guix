#!/bin/bash
#
# Install the build dependencies for the regression suite.
#
# This runs inside the pinned Debian container the workflow declares, as root,
# so there is no sudo and no hosted toolcache to clear.
#
# Versions are deliberately not taken from whatever is newest. The sources here
# declare cmake_minimum_required(VERSION 3.0.0), and CMake removed compatibility
# below 3.5 in 4.0, so a build that upgrades CMake stops configuring. Debian
# trixie carries 3.31.6, which is below 4.x, so the distribution package is the
# pin and nothing installs a newer one.
#
# The list is what the suite uses and nothing more. The tests launch no external
# program: the files they read and write are golden checksums, binary resources
# and text samples in this tree, opened by path. The runner and the coverage
# scripts use git, CMake, Ninja, gcovr and Python, and otherwise only coreutils,
# grep and sed, which the base image carries. The C library for -m32 comes with
# gcc-multilib. Every source is C, so no C++ compiler is installed.
# unifdef, p7zip-full and tofrodos are invoked by neither this tree nor the
# pinned harness: the .7z archives beside the golden files are never opened by
# the suite.

set -eux

apt-get update
apt-get install -y --no-install-recommends \
    ca-certificates \
    cmake \
    gcc \
    gcc-multilib \
    git \
    ninja-build \
    python3-pip

# gcovr reads a data format tied to the compiler, and the report it produces is
# certification evidence, so it is pinned rather than tracked. 8.6 is the version
# every flag used by the coverage scripts was verified against. Debian marks the
# system Python externally managed, hence --break-system-packages: the container
# is single-purpose and has no other Python software to break.
pip3 install --no-cache-dir --break-system-packages gcovr==8.6

gcc --version
cmake --version
ninja --version
gcovr --version

# The image tag is a moving target: Debian point releases rebuild it. The
# versions that matter are asserted rather than assumed, so a toolchain change
# fails here with a name and a number instead of quietly producing coverage
# evidence built with something else.
#
# GCC major only, because point releases within a major are bug-fix and the
# gcov data format is tied to the major. CMake below 4, because 4.0 dropped the
# compatibility these sources declare. gcovr exactly, because it is what the
# report is produced by.
gcc_major=$(gcc -dumpversion | cut -d. -f1)
cmake_major=$(cmake --version | head -1 | sed 's/[^0-9]*\([0-9]*\).*/\1/')
gcovr_version=$(gcovr --version | head -1 | awk '{print $2}')

[ "$gcc_major" = "14" ] || { echo "install.sh: expected GCC 14, got $(gcc -dumpfullversion)" >&2; exit 1; }
[ "$cmake_major" -lt 4 ] || { echo "install.sh: CMake 4.x cannot configure these sources, got $cmake_major" >&2; exit 1; }
[ "$gcovr_version" = "8.6" ] || { echo "install.sh: expected gcovr 8.6, got $gcovr_version" >&2; exit 1; }
