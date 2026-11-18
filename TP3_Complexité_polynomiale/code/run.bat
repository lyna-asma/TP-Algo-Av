@echo off
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
echo Compilation en cours...
gcc main.c -o main.exe
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ================= Execution =================
    main.exe
) else (
    echo.
    echo [ERREUR] La compilation a echoue.
)
