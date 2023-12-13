
To generate GUIX Studio installer, follow these steps:

1) Install Inno Setup from the following link:

http://www.jrsoftware.org/isinfo.php

2) Build GUIX Studio executable:
Execute script "build_guix_studio.cmd" from scripts folder.

3) Download Microsoft Visual C++ Redistributable:
Execute script "download_vc_redist.cmd" from scripts folder to download the Microsoft Visual C++ Redistributable package.
This package is essential for installing Microsoft C and C++(MSVC) runtime libraries.

4)Run the Inno Setup compiler:
Launch the Inno Setup compiler and open /GUIX/installer/guix_installer.iss.
Click the compiler button. When the compiler runs cleanly, the installer is generated in output folder.
