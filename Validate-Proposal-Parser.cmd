@echo off
setlocal
set "PUROOT=%~dp0"
where cl.exe >nul 2>nul
if errorlevel 1 (
 echo Open Developer PowerShell for VS 2022 in the repository, then run this command again.
 exit /b 1
)
set "PUSTAGE=%PUROOT%build\university\parser-%RANDOM%-%RANDOM%"
if exist "%PUSTAGE%" (
 echo Build staging directory already exists. Run again; no existing files were replaced.
 exit /b 1
)
mkdir "%PUSTAGE%"
if errorlevel 1 exit /b 1
pushd "%PUSTAGE%"
if errorlevel 1 exit /b 1
echo Checking the preserved F1 dependency...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\tests\proposal_tests.cpp" /Fe:proposal-tests.exe
if errorlevel 1 goto failed
proposal-tests.exe
if not "%ERRORLEVEL%"=="0" goto failed
echo Checking U4-F2 parser...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\tests\proposal_parser_tests.cpp" /Fe:proposal-parser-tests.exe
if errorlevel 1 goto failed
proposal-parser-tests.exe
if not "%ERRORLEVEL%"=="0" goto failed
echo Building University with U4-F2...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\src\Process.cpp" "%PUROOT%university\src\RepositoryActions.cpp" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\mentor\Channel.cpp" "%PUROOT%university\src\mentor\Mentor.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\src\ProposalReview.cpp" "%PUROOT%university\src\ReviewControls.cpp" "%PUROOT%university\src\StudyDesk.cpp" "%PUROOT%university\src\main.cpp" /Fe:pandata-university.exe
if errorlevel 1 goto failed
echo U4-F2 valid: expected exit 0
pandata-university.exe --proposal-json-fixture
if not "%ERRORLEVEL%"=="0" goto failed
echo U4-F2 malformed: expected exit 1
pandata-university.exe --proposal-json-malformed
if not "%ERRORLEVEL%"=="1" goto failed
echo U4-F2 stale: expected exit 1
pandata-university.exe --proposal-json-stale
if not "%ERRORLEVEL%"=="1" goto failed
echo U4-F2 fresh: expected exit 0
pandata-university.exe --proposal-json-fixture
if not "%ERRORLEVEL%"=="0" goto failed
echo PASS: U4-F2 valid / rejected / stale / valid; exits 0 / 1 / 1 / 0.
echo U1-U3 suites were not rerun. F1 was checked only as a parser dependency.
echo Build outputs are in "%PUSTAGE%". Runtime demos saved no progress or evidence.
popd
exit /b 0
:failed
echo FAIL: keep the first compiler, test, or unexpected-exit error above.
popd
exit /b 1
