@echo off
setlocal DisableDelayedExpansion
set "PUROOT=%~dp0"
if "%~1"=="" exit /b 2
if not "%~2"=="" exit /b 2
if not exist "%~1\" exit /b 2
pushd "%~1"
if errorlevel 1 exit /b 2
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\src\Process.cpp" "%PUROOT%university\src\RepositoryActions.cpp" "%PUROOT%university\src\StudyDesk.cpp" "%PUROOT%university\tests\final_integration_tests.cpp" /Fe:final-state.exe
set "PURESULT=%ERRORLEVEL%"
popd
exit /b %PURESULT%
