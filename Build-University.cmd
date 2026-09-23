@echo off
setlocal DisableDelayedExpansion
set "PUROOT=%~dp0"
if "%~1"=="" exit /b 2
if not "%~2"=="" exit /b 2
if not exist "%~1\" exit /b 2
where cl.exe >nul 2>nul
if errorlevel 1 exit /b 2
pushd "%~1"
if errorlevel 1 exit /b 2
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\src\Process.cpp" "%PUROOT%university\src\RepositoryActions.cpp" "%PUROOT%university\src\StudyDesk.cpp" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\mentor\Channel.cpp" "%PUROOT%university\src\mentor\Mentor.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\src\ProposalReview.cpp" "%PUROOT%university\src\ReviewControls.cpp" "%PUROOT%university\src\main.cpp" /Fe:pandata-university.exe
set "PUBUILDRESULT=%ERRORLEVEL%"
popd
exit /b %PUBUILDRESULT%
