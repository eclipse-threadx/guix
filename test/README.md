
# GUIX and GUIX Studio Regression Test Guide
## Introduction
This guide provides instructions on how to add GUIX and GUIX Studio regression tests and how to effectively debug failed test cases. Regression testing is essential for ensuring that new code changes do not introduce new bugs or regressions. By following the steps outlined in this guide, you can contribute to maintaining a stable and reliable codebase.

## Table of Contents
1. [GUIX Regression Test](#guix-regression-test)
2. [GUIX Studio Regression Test](#guix-studio-regression-test)

## GUIX Regression Test

### Directory Structure

The GUIX regression test is built on top of the CMake build system. The test suite is defined in the `cmake\CMakeLists.txt` file. The test cases for each test suite are defined in the `cmake\regression\CMakeLists.txt` file in the corresponding subdirectory. The test cases are implemented in the `regression_test\tests` directory.

Detailed directory structure is as follows:
- **cmake\CMakeLists.txt:** The CMake file that defines the build configurations and the test suites.
- **cmake\regression\CMakeLists.txt:** The CMake file that defines the test cases for each test suite.
- **coverage.sh:** The script that generates the coverage report.
- **run.sh:** The script that builds and runs the tests.
- **display_driver:** This directory hosts the display drivers used by the tests.
- **golden_files:** This directory contains golden files used for verification during the tests, aiding in the comparison of expected and actual outcomes.
- **regression_test\tests:** This directory contains the all GUIX regression tests.
- **regression_test\utility:** This directory hosts utility files used by the regression test.
- **regression_test\gx_show_canvas.exe:** A tool that displays GUIX canvas data that stored in a binary file.

### Types of Test Cases

1. **Test case with output:**
- In the test code, after each test point, a pecified area of the GUIX canvas is saved to a binary file named `<test_name>.bin`.
- A checksum value, calculated based on the cavans data, is saved to a file named `<test_name>.checksum`.
- The binary file is mainly used for checking the correctness of the test case visually.
- The checksum file enables faster verification during tests by comparing expected and actual checksum values. 

2. **Test case without output:**
- I this type of test code, the test code typically checks the API return status or the values of a variable to verify the correctness of the test case.

### Prerequisites
1. Linux environment.
2. Clone the GUIX repository.
3. Install required packages using script `install.sh` located in the `scripts` directory.

### Run Regression Test
1. Navigate to the `test\guix_test\cmake` directory.

2. To build and run all the tests, use the following commands.
```bash
./run.sh build all
./run.sh run all
```

3. To build and run a specific test suite, use the following commands.
```bash
./run.sh build <build_configuration_name>
./run.sh run <build_configuration_name>
```

The **test reports** for each build configuration will be generated in the `test\guix_test\cmake\build\<build_configuration_name>` directory, where `<build_configuration_name>` is the name of the build configuration set.

The **coverage report** for default_build_covearge will be generated in the `test\guix_test\cmake\coverage_report` directory. For other build configurations, no coverage report will be generated.

### Run One Test Case

Follow these steps to execute an individual test case:

1. Build the test using the instructions provided in the previous section.
2. Navigate to the `test\guix_test\cmake\build\<build_configuration_name>\regression` directory, where the test executables are generated.

3. Run the test without output, where the test name is ended with `no_output`.
```bash
./<test_name>
```

4. Run other tests with checksum vertification and specify the golden files path with the following command.
```bash
 ./<test_name> -checksum -gpath ../../../../golden_files/
 ```
Ensure to replace <test_name> with the actual name of the test case you want to run. 

### Debug Failed Test

- Debugging with cgdb.
1. Navigate to the `test\guix_test\cmake\build\<build_configuration_name>\regression` directory, where the test executables are generated. Start cgdb using the following command.
```bash
cgdb <test_name>
```
Ensure to replace <test_name> with the actual name of the test case you want to debug.

2. Set configuration parameter `-r` to run test without comparison.
```bash
set args -r
```

3. Set breakpoints as needed and run the test.

- Debugging by comparing output files.
1. Navigate to the `test\guix_test\cmake\build\<build_configuration_name>\regression\output_files` directory, where the failed output binary file is generated. The generated binary file is named `<test_name>_failures.bin`.

2. Use gx_show_canvas.exe located in `test\guix_test\regression_test` to display the content in the failed output binary file. Compare it with the conresponding golden file located in the `test\guix_test\golden_files` directory. For the usage of gx_show_canvas.exe, please refer to gx_show_canvas.md.



### Add New Test

Follow these steps to addd a new test:
1. Create Win32 Demo Project.
- Generate a Win32 demo project under the `test\example_internal` directory.
- Ensure the main source file is named as `demo_guix_<example_name>.c`.

2. Add Test Source File:
- Place the test source file in the `test\guix_test\regression_test\tests` directory.
- Name the test file start with `validation_guix`.

3. Implement the Test Case:
- In the test source file, include the necessary header files:
```c
#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
```

- Set test parameters:
```c
TEST_PARAM test_parameter = {
    "", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};
```

Definition of the TEST_PARAM structure:
```c
typedef struct TEST_PARAM_S 
{

    char *test_name;          /* Must be set */


    /* The following parameters defines the screen area to capture.
       If 0, capture the whole screen. */
       
    int x_start;   
    int y_start;
    int x_end;
    int y_end;
} TEST_PARAM;
```

- Define the main function:
```c
int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}
```

- Create the test thread:
```c
static VOID control_thread_entry(ULONG);

VOID tx_application_define(void *first_unused_memory)
{
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}
```

- Include the test example source code:
```c
#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_<example_name>.c"
```

The test example source code also contains a main function, The header file `gx_validation_wrapper.h` is used to replace the main function in the test example source code.

- Thread entry example for test case without output:
```c
static VOID control_thread_entry(ULONG input)
{
int               failed_tests = 0;
UINT              status;
GX_PROMPT        *prompt = &button_screen.button_screen_title_1;
GX_CONST GX_CHAR *text;

    gx_widget_hide(&button_screen); 

    status = gx_prompt_text_get(prompt, &text);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    if(failed_tests == 0) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
```

- Thread entry example for test case with output:
```c
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
GX_EVENT my_event;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_display_handle = 1;

    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Initialize the screen.");
    gx_validation_screen_refresh();

    /* Simulate a pen down event. */
    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = 207;
    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = 376;
    gx_system_event_send(&my_event);

    /* Simulate a pen up event. */
    my_event.gx_event_type = GX_EVENT_PEN_UP;
    gx_system_event_send(&my_event);

    /* Force a screen refresh to capture the canvas data or compare the checksum. */
    gx_validation_set_frame_id(frame_id++);
    gx_validation_set_frame_comment("Scroll the vertical list.");
    gx_validation_screen_refresh();

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
```

## GUIX Studio Regression Test

### Run Regression Test

### Debug Failed Test

### Add New Test