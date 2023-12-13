To build the libpng library, follow these steps:

1) Download the libpng source code from the following link:

https://sourceforge.net/projects/libpng/files/

2) Download reference zlib source code from either of the following links:

https://sourceforge.net/projects/libpng/files/
or
https://www.zlib.net/

3) Build libpng using Microsoft Visual C++:
   Navigate to "libpng/projects/vstudio", which contains a Microsot Visual C++(MSVC) workspace for building libpng and zlib.

4) According to MS policy, binaries should be compiled with a warning level that enables all critical security-relevant checks.
The provided libpng and zlib project disabled warnings 4244 and 4996, which triggers an issue in TSA binary check.
Configure the projects and make sure warnings 4244 and 4996 are not been disabled.

5) If you are using Visual Studio 2017 version 15.7 or higher version, it raises warning C5045, configure the projects to diable this warning.
Now you are able to build png and zlib library successfully!

