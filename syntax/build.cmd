@echo off
setlocal

echo Building cpp-cli-syntax...
echo.

msbuild cpp-cli-syntax.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build succeeded!
    echo ========================================
    exit /b 0
) else (
    echo.
    echo ========================================
    echo Build failed with error code %ERRORLEVEL%
    echo ========================================
    exit /b %ERRORLEVEL%
)
