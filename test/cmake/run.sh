#!/bin/bash

cd $(dirname $0)
[ -f .run.sh ] || ln -sf ../../externals/threadx/.build/cmake_bootstrap.sh .run.sh
./.run.sh $*