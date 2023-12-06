To build the libjpeg turbo library, follow these steps:

1) Clone git repo from the following link:

https://github.com/libjpeg-turbo/libjpeg-turbo/

2) Install CMake, which can be obtained from:

https://cmake.org/download/

3) Run the command:
cmake -G"Visual Studio 16" -A"Win32"

This command will produce a .sln and several vcproj files to build using Visual Studio IDE.

4) Build the the libjpeg-static library, this is the only one that is needed to link with Studio.