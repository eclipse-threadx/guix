#!/bin/bash

cd $(dirname $0)
[ -f .run.sh ] || ln -sf ../../externals/threadx/scripts/cmake_bootstrap.sh .run.sh

# find line that contains "ctest $parallel" in .run.sh
line=$(grep -n "ctest \$parallel" .run.sh | cut -d: -f1)

# Inser "--output-junit $1.xml" after the line
sed -i "${line}a \\\t--output-junit $1.xml" .run.sh

./.run.sh $*

#!/bin/bash