@ECHO OFF

ECHO INFO: Setting up the environment for IAR...

IF EXIST %AZRTOS_IAR_DIR% GOTO FOUND_IAR_DIR
SET AZRTOS_IAR_DIR="C:\Program Files\IAR Systems\Embedded Workbench 9.1"
IF EXIST %AZRTOS_IAR_DIR% GOTO FOUND_IAR_DIR
ECHO ERROR: IAR Embedded Workbench not found.
EXIT /B 1
:FOUND_IAR_DIR

SET AZRTOS_IAR_BUILD=%AZRTOS_IAR_DIR%\common\bin\IarBuild.exe
IF NOT EXIST %AZRTOS_IAR_BUILD% (
    ECHO ERROR: IarBuild.exe not found.
    EXIT /B 1
) ELSE (
    ECHO INFO: IarBuild.exe found at %AZRTOS_IAR_BUILD%
)

SET AZRTOS_IAR_CSPYBAT=%AZRTOS_IAR_DIR%\common\bin\CSpyBat.exe
IF NOT EXIST %AZRTOS_IAR_CSPYBAT% (
    ECHO ERROR: CSpyBat not found.
    EXIT /B 1
) ELSE (
    ECHO INFO: CSpyBat found at %AZRTOS_IAR_CSPYBAT%
)

ECHO INFO: IAR environment setup completed successfully!