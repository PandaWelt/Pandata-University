@echo off
setlocal DisableDelayedExpansion
if "%~1"=="" exit /b 2
if not "%~3"=="" exit /b 2
if not "%~2"=="" if not "%~2"=="--tests" exit /b 2
if "%~2"=="--tests" goto tests
powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0university\tools\Build-Desktop.ps1" -Repository "%~dp0." -Output "%~1"
exit /b %ERRORLEVEL%
:tests
powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0university\tools\Build-Desktop.ps1" -Repository "%~dp0." -Output "%~1" -Tests
exit /b %ERRORLEVEL%
