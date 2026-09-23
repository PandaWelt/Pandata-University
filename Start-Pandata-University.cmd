@echo off
setlocal DisableDelayedExpansion
set "PUROOT=%~dp0"
pushd "%PUROOT%"
if errorlevel 1 exit /b 1
call :tools
if errorlevel 1 goto failed
set "PUSTAGE=%PUROOT%build\university\launch-%RANDOM%-%RANDOM%"
if exist "%PUSTAGE%" goto failed
mkdir "%PUSTAGE%"
if errorlevel 1 goto failed
echo Building University. Regression suites are separate from ordinary study launch.
call "%PUROOT%Build-University.cmd" "%PUSTAGE%"
if errorlevel 1 goto failed
"%PUSTAGE%\pandata-university.exe" --validate
if errorlevel 1 goto failed
"%PUSTAGE%\pandata-university.exe"
if errorlevel 1 goto failed
echo Session ended. Responses remain in course\university\progress.
popd
pause
exit /b 0
:failed
echo University stopped safely. Keep the first error; no progress or source was reset.
popd
pause
exit /b 1
:tools
where cl.exe >nul 2>nul
if not errorlevel 1 exit /b 0
set "PUVSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%PUVSWHERE%" goto notools
set "PUVS="
for /f "usebackq tokens=*" %%I in (`"%PUVSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "PUVS=%%I"
if not defined PUVS goto notools
call "%PUVS%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 1
where cl.exe >nul 2>nul
exit /b %errorlevel%
:notools
echo Could not find the installed Visual Studio C++ compiler.
echo Try this launcher from Developer Command Prompt for VS 2022, or check the Desktop development with C++ workload.
exit /b 1
