#!/bin/bash
CTEST_PARALLEL_LEVEL=4 $(dirname `realpath $0`)/../test/guix_test/cmake/run.sh test all
