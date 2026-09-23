@echo off
setlocal DisableDelayedExpansion
if not "%~1"=="" exit /b 2
set "PUROOT=%~dp0"
set "PUEXE=%PUROOT%build\university\desktop\pandata-university-desktop.exe"
call :tools
if exist "%PUEXE%" goto launch
if errorlevel 1 goto failed
call "%PUROOT%Build-University-Desktop.cmd" "%PUROOT%build\university\desktop"
if errorlevel 1 goto failed
:launch
start "" /D "%PUROOT%." "%PUEXE%" --repository "%PUROOT%."
exit /b %ERRORLEVEL%
:failed
echo Desktop build stopped. Keep the first error. Start-Pandata-University.cmd remains the console fallback.
pause
exit /b 1
:tools
where cl.exe >nul 2>nul
if not errorlevel 1 exit /b 0
set "PUVSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%PUVSWHERE%" exit /b 1
set "PUVS="
for /f "usebackq tokens=*" %%I in (`"%PUVSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "PUVS=%%I"
if not defined PUVS exit /b 1
call "%PUVS%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if errorlevel 1 exit /b 1
where cl.exe >nul 2>nul
exit /b %ERRORLEVEL%
