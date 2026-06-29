@echo off
setlocal

echo Running tests for cpp-cli-syntax...
echo.

REM Try to find vstest.console.exe (VS 2026 or VS 2022)
set VSTEST_PATH=
if exist "C:\Program Files\Microsoft Visual Studio\18\Enterprise\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" (
    set "VSTEST_PATH=C:\Program Files\Microsoft Visual Studio\18\Enterprise\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" (
    set "VSTEST_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe"
) else (
    echo ERROR: Could not find vstest.console.exe
    echo Please ensure Visual Studio 2022 or 2026 is installed with the test platform
    exit /b 1
)

echo Using: %VSTEST_PATH%
echo.

REM Check if the test assembly exists
set TEST_DLL=
if exist "syntax\Debug\cpp-cli-syntax.dll" (
    set "TEST_DLL=syntax\Debug\cpp-cli-syntax.dll"
) else (
    echo ERROR: cpp-cli-syntax.dll not found in syntax\Debug
    echo Please run build.cmd first
    exit /b 1
)

echo Testing: %TEST_DLL%
echo.

REM Run the tests
"%VSTEST_PATH%" "%TEST_DLL%" /Logger:console;verbosity=normal

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo All tests passed!
    echo ========================================
    exit /b 0
) else (
    echo.
    echo ========================================
    echo Tests failed with error code %ERRORLEVEL%
    echo ========================================
    exit /b %ERRORLEVEL%
)
