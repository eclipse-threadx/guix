@ECHO OFF
setlocal enabledelayedexpansion

SET LIBRARY_LIST=tx gx
SET TEST_LIST=demo_guix_jpeg_decode_565rgb demo_guix_jpeg_decode_565rgb_rotated_ccw demo_guix_jpeg_decode_565rgb_rotated_cw demo_guix_jpeg_decode_1555xrgb demo_guix_jpeg_decode_24xrgb demo_guix_jpeg_decode_24xrgb_rotated_cw demo_guix_jpeg_decode_8bit_palette
SET PROJECT_LIST=%LIBRARY_LIST% %TEST_LIST%
SET CONFIGURATION_LIST=Debug Debug_Helium

call setenv.bat

IF %ERRORLEVEL% NEQ 0 (
    EXIT /B %ERRORLEVEL%
)

call clean.bat

IF %ERRORLEVEL% NEQ 0 (
    EXIT /B %ERRORLEVEL%
)

call build.bat

IF %ERRORLEVEL% NEQ 0 (
    EXIT /B %ERRORLEVEL%
)

SET PROC_DLL=%AZRTOS_IAR_DIR%\arm\bin\armPROC.dll
SET DRIVER_DLL=%AZRTOS_IAR_DIR%\arm\bin\armSIM2.dll
SET PLUGIN=%AZRTOS_IAR_DIR%\arm\bin\armbat.dll

:: Loop through configuration list.
FOR %%c IN (%CONFIGURATION_LIST%) DO (
    ECHO INFO: Testing configuration %%c...

    FOR %%i IN (%TEST_LIST%) DO (
        ECHO INFO: Test %%i - %%c...

        :: If output_results.c exists, delete it.
        IF EXIST output_results.c (
            DEL output_results.c
        )

        :: Run test
        SET DEBUG_FILE=%%i\%%c\Exe\%%i.out

        IF NOT EXIST !DEBUG_FILE! (
            ECHO ERROR: !DEBUG_FILE! not found.
            EXIT /B 4
        )

        %AZRTOS_IAR_CSPYBAT% %PROC_DLL% %DRIVER_DLL% !DEBUG_FILE! --macro "setup.mac" --plugin=%PLUGIN%  --backend -f "test.driver.xcl"

        IF !ERRORLEVEL! NEQ 0 (
            ECHO ERROR: %%i.ewp not found.
            EXIT /B 4
        )

        :: Check if output_results.c exists.
        IF NOT EXIST output_results.c (
            ECHO ERROR: output_results.c not found.
            EXIT /B 4
        )

        :: Compare output_results.c with golden_results.c
        FC %%i\golden_results.c output_results.c
        IF !ERRORLEVEL! NEQ 0 (
            ECHO ERROR: unexpected test results.
            EXIT /B 4
        )
    )
)

ECHO INFO: Test completed successfully!
