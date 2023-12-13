Notes on Studio test

The test script is written using Python. In order to run this test suite,
you must first install Python for Windows version 3.0 or higher.

1) Install Python version 3.0 or later. There are several versions so
   make sure you install the right one. Make sure you install the 64-bit
   version if you have a 64-bit OS. You can find it here:

    http://www.python.org/getit/

2) Run the test script.

    From a command line prompt:

    >> python output_files_cmp.py

You should see Studio be compiled first, the produced studio executable file under "Release" directory
will be used to generate output files of all gxp projects under trunk except projects that specified
in ignore_list file. The generated files then be verified with the old ones. Test results will comes into
"result.txt" file.
