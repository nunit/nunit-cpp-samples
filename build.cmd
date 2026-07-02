@echo off
setlocal

REM Locate Visual Studio MSBuild (required for C++/CLI projects)
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set MSBUILD=%%i
)

if not defined MSBUILD (
    echo ERROR: Visual Studio MSBuild not found. Please install Visual Studio with C++ workload.
    exit /b 1
)

echo Restoring NuGet packages...
echo.

"%MSBUILD%" syntax\cpp-cli-syntax.vcxproj /t:Restore /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ========================================
    echo NuGet restore failed with error code %ERRORLEVEL%
    echo ========================================
    exit /b %ERRORLEVEL%
)

echo.
echo Building cpp-cli-syntax...
echo.

"%MSBUILD%" syntax\cpp-cli-syntax.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ========================================
    echo Build failed with error code %ERRORLEVEL%
    echo ========================================
    exit /b %ERRORLEVEL%
)

REM Always copy runtime dependencies (post-build skipped on incremental builds)
copy /Y "%USERPROFILE%\.nuget\packages\nunit3testadapter\6.2.0\build\net8.0\*.dll" "syntax\Debug\" >nul
copy /Y "%USERPROFILE%\.nuget\packages\nunit\5.0.0-alpha.100.22\lib\net8.0\nunit.framework.legacy.dll" "syntax\Debug\" >nul
copy /Y "%USERPROFILE%\.nuget\packages\system.security.permissions\9.0.0\lib\net8.0\System.Security.Permissions.dll" "syntax\Debug\" >nul
copy /Y "%USERPROFILE%\.nuget\packages\microsoft.extensions.dependencymodel\8.0.2\lib\net8.0\Microsoft.Extensions.DependencyModel.dll" "syntax\Debug\" >nul

echo.
echo ========================================
echo Build succeeded!
echo ========================================
exit /b 0
