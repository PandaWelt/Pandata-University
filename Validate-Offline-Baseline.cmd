@echo off
setlocal DisableDelayedExpansion
powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0university\tools\Validate-Offline-Baseline.ps1" -Repository "%~dp0."
exit /b %ERRORLEVEL%
