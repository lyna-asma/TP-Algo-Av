@echo off
set "PATH=C:\msys64\ucrt64\bin;%PATH%"
echo Compilation de Tri_Bulles.c ...
gcc Tri_Bulles.c -o Tri_Bulles.exe
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ================= Execution =================
    Tri_Bulles.exe
) else (
    echo.
    echo [ERREUR] La compilation a echoue.
)
