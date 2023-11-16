@ECHO OFF

FOR %%c IN (%CONFIGURATION_LIST%) DO (
    FOR %%i IN (%PROJECT_LIST%) DO (
        IF NOT EXIST %%i\%%i.ewp (
            ECHO ERROR: %%i.ewp not found.
            EXIT /B 3
        )

        ECHO INFO: Building %%i...

        IF %%i == tx (
            %AZRTOS_IAR_BUILD% %%i\%%i.ewp -make Debug -log errors   
        ) ELSE (
            %AZRTOS_IAR_BUILD% %%i\%%i.ewp -make %%c -log errors
        )

        IF %ERRORLEVEL% NEQ 0 (
            ECHO ERROR: failed to build %%i.ewp
            EXIT /B 3
        )
    )
)

ECHO INFO: Build completed successfully!