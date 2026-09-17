@echo off
setlocal
set "PUROOT=%~dp0"
pushd "%PUROOT%"
if errorlevel 1 exit /b 1
call :tools
if errorlevel 1 goto failed
set "PUSTAGE=build\university\stage-%RANDOM%-%RANDOM%"
mkdir "%PUSTAGE%"
if errorlevel 1 goto failed
pushd "%PUSTAGE%"
if errorlevel 1 goto failed
echo Building Pandata University checks...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\tests\engine_tests.cpp" /Fe:engine-tests.exe
if errorlevel 1 goto buildfailed
engine-tests.exe "%PUROOT%course\university\missions\u1-boundaries.pum" "%PUROOT%course\university\pilot.puc"
if errorlevel 1 goto buildfailed
echo Building Pandata University...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\src\main.cpp" /Fe:pandata-university.exe
if errorlevel 1 goto buildfailed
popd
"%PUSTAGE%\pandata-university.exe" --validate
if errorlevel 1 goto failed
"%PUSTAGE%\pandata-university.exe"
if errorlevel 1 goto failed
echo Session ended. Any recorded responses remain in course\university\progress.
popd
pause
exit /b 0
:buildfailed
popd
:failed
echo Pandata University did not start or complete successfully. Keep the first error above for review.
echo Your PandaBank source and Git history were not changed by this launcher.
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
