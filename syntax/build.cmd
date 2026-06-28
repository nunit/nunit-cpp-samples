@echo off
setlocal

echo Building cpp-cli-syntax...
echo.

msbuild cpp-cli-syntax.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ========================================
    echo Build failed with error code %ERRORLEVEL%
    echo ========================================
    exit /b %ERRORLEVEL%
)

REM Always copy runtime dependencies (post-build skipped on incremental builds)
copy /Y "C:\Users\TerjeSandstrom\.nuget\packages\nunit3testadapter\6.2.0\build\net8.0\*.dll" "Debug\" >nul
copy /Y "C:\Users\TerjeSandstrom\.nuget\packages\nunit\5.0.0-alpha.100.20\lib\net8.0\nunit.framework.legacy.dll" "Debug\" >nul
copy /Y "C:\Users\TerjeSandstrom\.nuget\packages\system.security.permissions\9.0.0\lib\net8.0\System.Security.Permissions.dll" "Debug\" >nul
copy /Y "C:\Users\TerjeSandstrom\.nuget\packages\microsoft.extensions.dependencymodel\8.0.0\lib\net8.0\Microsoft.Extensions.DependencyModel.dll" "Debug\" >nul

echo.
echo ========================================
echo Build succeeded!
echo ========================================
exit /b 0
