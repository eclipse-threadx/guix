#!/bin/bash

cd $(dirname $0)

# find line that contains "ctest $parallel" in .run.sh
line=$(grep -n "ctest \$parallel" ../../externals/threadx/scripts/cmake_bootstrap.sh | cut -d: -f1)

# Add "--output-junit $1.xml" to the end of the line
sed -i "${line}s/$/ --output-junit $1.xml/" ../../externals/threadx/scripts/cmake_bootstrap.sh

[ -f .run.sh ] || ln -sf ../../externals/threadx/scripts/cmake_bootstrap.sh .run.sh
./.run.sh $*

#!/bin/bash