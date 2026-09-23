@echo off
setlocal DisableDelayedExpansion
set "PUROOT=%~dp0"
where cl.exe >nul 2>nul
if errorlevel 1 (
 echo Open Developer PowerShell for VS 2022 in the repository, then run this command again.
 exit /b 1
)
set "PUSTAGE=%PUROOT%build\university\offline-%RANDOM%-%RANDOM%"
if exist "%PUSTAGE%" (
 echo Build staging directory already exists. Run again; no existing files were replaced.
 exit /b 1
)
mkdir "%PUSTAGE%"
if errorlevel 1 exit /b 1
echo Validation staging: "%PUSTAGE%"
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
echo Checking U4-F3 review session...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\src\ProposalReview.cpp" "%PUROOT%university\tests\proposal_review_tests.cpp" /Fe:proposal-review-tests.exe
if errorlevel 1 goto failed
proposal-review-tests.exe
if not "%ERRORLEVEL%"=="0" goto failed
echo Checking U4-F4 controls...
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\src\ProposalReview.cpp" "%PUROOT%university\src\ReviewControls.cpp" "%PUROOT%university\tests\review_controls_tests.cpp" /Fe:review-controls-tests.exe
if errorlevel 1 goto failed
review-controls-tests.exe
if not "%ERRORLEVEL%"=="0" goto failed
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\mentor\Json.cpp" "%PUROOT%university\src\ProposalPreview.cpp" "%PUROOT%university\src\ProposalParser.cpp" "%PUROOT%university\src\ProposalReview.cpp" "%PUROOT%university\src\ReviewControls.cpp" "%PUROOT%university\tests\command_boundary_tests.cpp" /Fe:command-boundary-tests.exe
if errorlevel 1 goto failed
command-boundary-tests.exe
if not "%ERRORLEVEL%"=="0" goto failed
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 /I"%PUROOT%university\include" "%PUROOT%university\src\Mission.cpp" "%PUROOT%university\src\ProgressStore.cpp" "%PUROOT%university\src\Process.cpp" "%PUROOT%university\src\RepositoryActions.cpp" "%PUROOT%university\src\StudyDesk.cpp" "%PUROOT%university\tests\study_tests.cpp" /Fe:study-tests.exe
if errorlevel 1 goto failed
study-tests.exe "%PUROOT%."
if not "%ERRORLEVEL%"=="0" goto failed
call "%PUROOT%Build-University.cmd" "%PUSTAGE%"
if errorlevel 1 goto failed
echo U4-F4 demo: expected exit 0
pandata-university.exe --review-controls-demo
if not "%ERRORLEVEL%"=="0" goto failed
echo U4-F4 refusal: expected exit 1
pandata-university.exe --review-controls-refusal
if not "%ERRORLEVEL%"=="1" goto failed
echo U4-F4 demo: expected exit 0
pandata-university.exe --review-controls-demo
if not "%ERRORLEVEL%"=="0" goto failed
cl.exe /nologo /std:c++20 /EHsc /W4 /utf-8 "%PUROOT%university\tests\console_input_probe.cpp" /Fe:console-input-probe.exe
if errorlevel 1 goto failed
echo Checking actual University console routing...
pushd "%PUROOT%"
if errorlevel 1 goto failed
rem BEGIN DETERMINISTIC CONSOLE INPUT
> "%PUSTAGE%\console-input.txt" echo(review-status
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(review-select
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(review-preview
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(review-apply
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(review-dismiss
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(review-preview
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(review-status
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\console-input.txt" echo(quit
if errorlevel 1 goto inputfailed
rem END DETERMINISTIC CONSOLE INPUT
"%PUSTAGE%\console-input-probe.exe" --verify < "%PUSTAGE%\console-input.txt" > "%PUSTAGE%\input-evidence.json"
if errorlevel 1 goto inputfailed
echo PASS: exact 8-command CRLF input verified.
copy /b "%PUSTAGE%\console-input.txt" "%PUSTAGE%\mismatch-input.txt" >nul
if errorlevel 1 goto inputfailed
>> "%PUSTAGE%\mismatch-input.txt" echo(
if errorlevel 1 goto inputfailed
"%PUSTAGE%\console-input-probe.exe" --verify < "%PUSTAGE%\mismatch-input.txt" > "%PUSTAGE%\mismatch-evidence.json"
if not "%ERRORLEVEL%"=="1" goto inputfailed
"%PUSTAGE%\console-input-probe.exe" --verify < "%PUSTAGE%\console-input.txt" > "%PUSTAGE%\fresh-input-evidence.json"
if not "%ERRORLEVEL%"=="0" goto inputfailed
"%PUSTAGE%\pandata-university.exe" < "%PUSTAGE%\console-input.txt" > "%PUSTAGE%\console.txt"
set "PUCONSOLEEXIT=%ERRORLEVEL%"
popd
if not "%PUCONSOLEEXIT%"=="0" goto failed
findstr /L /C:"REVIEW STATUS: empty" "%PUSTAGE%\console.txt" >nul
if errorlevel 1 goto failed
findstr /L /C:"DISPLAYED: U4-F1" "%PUSTAGE%\console.txt" >nul
if errorlevel 1 goto failed
findstr /L /C:"REJECTED: use exactly" "%PUSTAGE%\console.txt" >nul
if errorlevel 1 goto failed
findstr /L /C:"REVIEW STATUS: dismissed" "%PUSTAGE%\console.txt" >nul
if errorlevel 1 goto failed
findstr /L /C:"NO ACTIVE PROPOSAL:" "%PUSTAGE%\console.txt" >nul
if errorlevel 1 goto failed
echo Checking actual offline study and relaunch in disposable staging...
study-tests.exe --prepare "%PUROOT%." "%PUSTAGE%"
if errorlevel 1 goto failed
pushd "%PUSTAGE%\workflow"
if errorlevel 1 goto failed
"%PUSTAGE%\pandata-university.exe" < "%PUSTAGE%\study-first.txt" > "%PUSTAGE%\study-first-output.txt"
set "PUFIRST=%ERRORLEVEL%"
"%PUSTAGE%\pandata-university.exe" < "%PUSTAGE%\study-resume.txt" > "%PUSTAGE%\study-resume-output.txt"
set "PUSECOND=%ERRORLEVEL%"
popd
if not "%PUFIRST%"=="0" goto failed
if not "%PUSECOND%"=="0" goto failed
study-tests.exe --verify "%PUSTAGE%"
if errorlevel 1 goto failed
echo PASS: combined offline foundation; retained regressions, study desk, input refusal and console restart passed.
echo This is Windows run 1 of 2. U1-U3 suites were not rerun. No real learner progress was written.
echo Keep the staging logs: "%PUSTAGE%"
popd
exit /b 0
:inputfailed
popd
goto failed
:failed
echo FAIL: keep the first error and staging logs: "%PUSTAGE%"
popd
exit /b 1
