@echo off
setlocal DisableDelayedExpansion
rem Diagnostic ONLY: grouped pipelines feed a bounded byte probe, NEVER University.
rem The production validator uses deterministic files and exact byte verification.
set "PROBESTAGE=%~1"
if not exist "%PROBESTAGE%\console-input-probe.exe" exit /b 2
(echo review-status& echo review-select& echo review-preview& echo review-apply& echo review-dismiss& echo review-preview& echo review-status& echo quit) | "%PROBESTAGE%\console-input-probe.exe" --diagnose > "%PROBESTAGE%\original-pipeline.json"
if errorlevel 1 exit /b 2
(echo(review-status& echo(review-select& echo(review-preview& echo(review-apply& echo(review-dismiss& echo(review-preview& echo(review-status& echo(quit) | "%PROBESTAGE%\console-input-probe.exe" --diagnose > "%PROBESTAGE%\echo-parenthesis-pipeline.json"
if errorlevel 1 exit /b 2
echo Captured old pipeline bytes in original-pipeline.json and echo-parenthesis-pipeline.json.
exit /b 0
