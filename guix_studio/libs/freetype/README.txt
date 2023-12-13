
To build the freetype library, follow these steps:

1) Download the FreeType source code from the following link:

https://sourceforge.net/projects/freetype/files/freetype2/

2) Build FreeType using Microsoft Visual C++:
Navigate to the "build/windows/vc2010" directory, which contains a Microsoft Visual C++ (MSVC) workspace for constructing the FreeType library.

3) Within the FreeType library, a customized version of the zlib library is present. To prevent compatibility issues with the separately included zlib library in GUIX Studio, it's better to rename all types and functions associated with the zlib library. The file "ftzconf.h" in the FreeType library facilitates this process by enabling the redefinition of types and library functions using a unique prefix, achieved through the Z_PREFIX configuration.

Despite implementing these changes, certain link errors may persist. To mitigate this, additional defines are necessary:

#define crc32_combine_gen     z_crc32_combine_gen
#define crc32_combine_gen64   z_crc32_combine_gen64
#define crc32_combine_op      z_crc32_combine_op
#define zcalloc               z_zcalloc
#define zcfree                z_zcfree
#define z_errmsg              z_z_errmsg

4) Following these steps, you should have successfully built the FreeType library.