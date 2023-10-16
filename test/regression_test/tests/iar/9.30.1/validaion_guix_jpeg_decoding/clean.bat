@ECHO OFF

FOR %%c IN (%CONFIGURATION_LIST%) DO (
    FOR %%i IN (%PROJECT_LIST%) DO (
        IF NOT EXIST %%i\%%i.ewp (
            ECHO ERROR: %%i.ewp not found.
            EXIT /B 2
        )

        ECHO INFO: Cleaning %%i...

        IF %%i == tx (
            %AZRTOS_IAR_BUILD% %%i\%%i.ewp -clean Debug -log errors   
        ) ELSE (
            %AZRTOS_IAR_BUILD% %%i\%%i.ewp -clean %%c -log errors
        )

        IF %ERRORLEVEL% NEQ 0 (
            ECHO ERROR: clean failed.
            EXIT /B 2
        )
    )
)

ECHO INFO: Clean completed successfully!