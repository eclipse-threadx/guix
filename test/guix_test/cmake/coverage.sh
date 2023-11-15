#!/bin/bash

set -e

cd $(dirname $0)
mkdir -p coverage_report/$1
gcovr --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --xml-pretty --output coverage_report/$1.xml
gcovr --object-directory=build/$1/guix/CMakeFiles/guix.dir/common/src -r ../../../common/src --html --html-details --output coverage_report/$1/index.html
