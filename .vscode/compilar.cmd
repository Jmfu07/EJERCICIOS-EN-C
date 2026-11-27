@echo off
setlocal EnableDelayedExpansion

REM ============================================================================
REM Compilador portable para VS Code (multicarpeta)
REM Uso: compilar.cmd <auto|msys2|tdm> <archivo.c> <salida.exe>
REM Busca primero en .\tools\ (USB/portable), luego rutas comunes y PATH.
REM ============================================================================

set "MODO=%~1"
set "SRC=%~2"
set "OUT=%~3"
set "ROOT=%~dp0.."
set "GCC="

if /I "%MODO%"=="msys2" call :BUSCAR_MSYS2
if /I "%MODO%"=="tdm"   call :BUSCAR_TDM
if /I "%MODO%"=="auto" (
    call :BUSCAR_MSYS2
    if not defined GCC call :BUSCAR_TDM
    if not defined GCC call :BUSCAR_PATH
)

if not defined GCC (
    echo [ERROR] No se encontro el compilador "%MODO%".
    echo         Instala MSYS2 o TDM-GCC, agregalo al PATH, o copialo en:
    echo           %ROOT%\tools\msys2\ucrt64\bin\gcc.exe
    echo           %ROOT%\tools\tdm-gcc\bin\gcc.exe
    exit /b 1
)

for %%D in ("%GCC%") do set "BIN_DIR=%%~dpD"
set "PATH=%BIN_DIR%;%PATH%"

echo [COMPILADOR %MODO%] %GCC%
"%GCC%" -fdiagnostics-color=always -g -Wall -Wextra -static-libgcc "%SRC%" -o "%OUT%" -lm
exit /b %ERRORLEVEL%

:BUSCAR_MSYS2
for %%P in (
    "%ROOT%\tools\msys2\ucrt64\bin\gcc.exe"
    "%ROOT%\tools\msys2\mingw64\bin\gcc.exe"
    "C:\LENGUAJES\MSYS2\ucrt64\bin\gcc.exe"
    "C:\LENGUAJES\MSYS2\mingw64\bin\gcc.exe"
    "C:\msys64\ucrt64\bin\gcc.exe"
    "C:\msys64\mingw64\bin\gcc.exe"
) do (
    if not defined GCC if exist "%%~P" set "GCC=%%~P"
)
if not defined GCC (
    for /f "delims=" %%W in ('where gcc 2^>nul') do (
        echo %%W | findstr /I "msys ucrt64 mingw64" >nul && if not defined GCC set "GCC=%%W"
    )
)
exit /b 0

:BUSCAR_TDM
for %%P in (
    "%ROOT%\tools\tdm-gcc\bin\gcc.exe"
    "C:\LENGUAJES\TDM-GCC\bin\gcc.exe"
    "C:\TDM-GCC-64\bin\gcc.exe"
    "C:\Program Files (x86)\Dev-Cpp\TDM-GCC-64\bin\gcc.exe"
    "C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe"
    "C:\Dev-Cpp\TDM-GCC-64\bin\gcc.exe"
) do (
    if not defined GCC if exist "%%~P" set "GCC=%%~P"
)
if not defined GCC (
    for /f "delims=" %%W in ('where gcc 2^>nul') do (
        echo %%W | findstr /I "tdm dev-cpp" >nul && if not defined GCC set "GCC=%%W"
    )
)
exit /b 0

:BUSCAR_PATH
for /f "delims=" %%W in ('where gcc 2^>nul') do (
    if not defined GCC set "GCC=%%W"
)
exit /b 0
