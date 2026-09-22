@echo off
setlocal
set "PUROOT=%~dp0"
where cl.exe >nul 2>nul
if errorlevel 1 (
 echo Open Developer PowerShell for VS 2022 in the repository, then run this command again.
 exit /b 1
)
set "PUSTAGE=%PUROOT%build\university\preview-%RANDOM%-%RANDOM%"
if exist "%PUSTAGE%" (
 echo Build staging directory already exists. Run again; no existing files were replaced.
 exit /b 1
)
mkdir "%PUSTAGE%"
if errorlevel 1 exit /b 1
pushd "%PUSTAGE%"
if errorlevel 1 exit /b 1
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\tests\proposal_tests.cpp" /Fe:proposal-tests.exe
if errorlevel 1 goto failed
proposal-tests.exe
if errorlevel 1 goto failed
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\src\Process.cpp" "%PUROOT%university\src\RepositoryActions.cpp" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\mentor\Channel.cpp" "%PUROOT%university\src\mentor\Mentor.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\src\ProposalReview.cpp" "%PUROOT%university\src\main.cpp" /Fe:pandata-university.exe
if errorlevel 1 goto failed
pandata-university.exe --proposal-fixture
if errorlevel 1 goto failed
pandata-university.exe --proposal-stale-fixture
if not "%ERRORLEVEL%"=="1" goto failed
pandata-university.exe --proposal-fixture
if errorlevel 1 goto failed
echo PASS: preview / expected stale rejection / preview. U1-U3 checks were not rerun.
echo Build outputs are in "%PUSTAGE%". The preview saved no progress or evidence.
popd
exit /b 0
:failed
echo FAIL: keep the first compiler or fixture error above.
popd
exit /b 1
