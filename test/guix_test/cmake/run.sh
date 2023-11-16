#!/bin/bash

cd $(dirname $0)

# if threadx repo does not exist, clone it
[ -d threadx ] ||
git clone https://github.com/azure-rtos/threadx.git

# find line that contains "ctest $parallel" in .run.sh
line=$(grep -n "ctest \$parallel" threadx/scripts/cmake_bootstrap.sh | cut -d: -f1)

# Add "--output-junit $1.xml" to the end of the line
sed -i "${line}s/$/ --output-junit $1.xml/" threadx/scripts/cmake_bootstrap.sh

[ -f .run.sh ] || ln -sf threadx/scripts/cmake_bootstrap.sh .run.sh
./.run.sh $*

#!/bin/bash