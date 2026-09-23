param([Parameter(Mandatory=$true)][string]$Repository)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
# Trusted maintainer harness. All sessions/processes run in a fresh disposable copy.
# Real source/history/progress are read only to copy reviewed inputs or hash preservation.
function Need([bool]$yes, [string]$why) { if (!$yes) { throw $why } }
function NoReparse([string]$path) {
    for ($p = [IO.Path]::GetFullPath($path); $p; $p = [IO.Path]::GetDirectoryName($p)) {
        if (Test-Path -LiteralPath $p) {
            Need (((Get-Item -Force -LiteralPath $p).Attributes -band [IO.FileAttributes]::ReparsePoint) -eq 0) "Reparse path refused: $p"
        }
    }
}
function Map([string]$root, [string[]]$names) {
    $result = @{}
    foreach ($name in $names) {
        $path = Join-Path $root $name
        NoReparse $path
        if (!(Test-Path -LiteralPath $path)) { continue }
        $items = @(Get-Item -Force -LiteralPath $path)
        if ($items[0].PSIsContainer) { $items = @(Get-ChildItem -Force -Recurse -LiteralPath $path) }
        foreach ($item in $items) {
            NoReparse $item.FullName
            if (!$item.PSIsContainer) {
                $key = $item.FullName.Substring($root.Length).TrimStart('\','/').Replace('\','/')
                $result[$key] = (Get-FileHash -Algorithm SHA256 -LiteralPath $item.FullName).Hash.ToLowerInvariant()
            }
        }
    }
    return $result
}
function Digest($map) {
    $lines = @($map.Keys | Sort-Object | ForEach-Object { $_ + ':' + $map[$_] })
    $sha = [Security.Cryptography.SHA256]::Create()
    try { return ([BitConverter]::ToString($sha.ComputeHash([Text.Encoding]::UTF8.GetBytes(($lines -join "`n"))))).Replace('-','').ToLowerInvariant() }
    finally { $sha.Dispose() }
}
function Same($a, $b, [string]$why) { Need (($a.Count -eq $b.Count) -and ((Digest $a) -eq (Digest $b))) $why }
function InvokeTest([string]$name, [string]$exe, [string]$arguments, [string]$cwd, [string]$inputFile = '', [int]$expected = 0) {
    $out = Join-Path $stage ($name + '.stdout.txt'); $err = Join-Path $stage ($name + '.stderr.txt')
    $options = @{FilePath=$exe; WorkingDirectory=$cwd; PassThru=$true; NoNewWindow=$true; RedirectStandardOutput=$out; RedirectStandardError=$err}
    if ($arguments) { $options['ArgumentList'] = $arguments }
    if ($inputFile) { $options['RedirectStandardInput'] = $inputFile }
    $p = Start-Process @options
    $null = $p.Handle # Retain native handle so ExitCode remains available after process exit.
    $clock = [Diagnostics.Stopwatch]::StartNew()
    while (!$p.WaitForExit(500)) {
        $size = (Get-Item -LiteralPath $out).Length + (Get-Item -LiteralPath $err).Length
        if ($clock.Elapsed.TotalSeconds -gt 600 -or $size -gt 8MB) {
            & "$env:SystemRoot\System32\taskkill.exe" /PID $p.Id /T /F | Out-Null
            throw "Bound exceeded: $name. Logs preserved."
        }
    }
    $p.WaitForExit()
    Need ($null -ne $p.ExitCode) "Missing exit status: $name"
    Need (((Get-Item $out).Length + (Get-Item $err).Length) -le 8MB) "Output bound exceeded: $name"
    $receipt.runs += [ordered]@{name=$name; executable=$exe; arguments=$arguments; exit=$p.ExitCode; expected=$expected; stdout=$out; stderr=$err}
    Need ($p.ExitCode -eq $expected) "$name exited $($p.ExitCode), expected $expected. See $out and $err"
    if ($expected) { Write-Host "EXPECTED REFUSAL: $name (exit $expected)" } else { Write-Host "PASS: $name (exit 0)" }
    return [IO.File]::ReadAllText($out) + [IO.File]::ReadAllText($err)
}
function Batch([string]$name, [string]$script, [string]$arguments = '', [string]$inputFile = '', [int]$expected = 0) {
    return InvokeTest $name "$env:SystemRoot\System32\cmd.exe" ('/d /v:off /s /c ""' + $script + '" ' + $arguments + '"') $copy $inputFile $expected
}
function InputFile([string]$name) {
    $path = Join-Path $stage ($name + '.input.txt')
    [IO.File]::WriteAllText($path, (($workflow.$name -join "`r`n") + "`r`n"), (New-Object Text.UTF8Encoding($false)))
    return $path
}

$root = (Resolve-Path -LiteralPath $Repository).Path.TrimEnd('\','/')
NoReparse $root
Need ($root -notmatch '[%"\r\n]') 'Validation refuses percent/quote/newline in repository path; do not reinterpret cmd metacharacters.'
Need (Test-Path -LiteralPath (Join-Path $root '.git') -PathType Container) 'Expected ordinary repository .git directory, not a linked worktree.'
$null = Get-Command cl.exe -ErrorAction Stop
$git = (Get-Command git.exe -ErrorAction Stop).Source
$stage = Join-Path $root ('build\university\final-' + [guid]::NewGuid().ToString('N'))
NoReparse $stage
$null = New-Item -ItemType Directory -Path $stage
$copy = Join-Path $stage 'learner space & probe'
$null = New-Item -ItemType Directory -Path $copy
Write-Host "Windows run 2 of 2. Validation staging: $stage"
$protected = @('src','.git','course','university','.gitignore') + @(Get-ChildItem -LiteralPath $root -Filter '*.cmd' | ForEach-Object Name)
# Native project definitions, never IDE caches or compiled output.
$protected += @(Get-ChildItem -File -LiteralPath (Join-Path $root 'visualstudio') | Where-Object { $_.Name -match '\.(vcxproj|filters|sln|slnx)$' } | ForEach-Object { 'visualstudio/'+$_.Name })
$before = Map $root $protected
$receipt = [ordered]@{baseline='PU-OFFLINE-1.0'; windowsRun=2; passed=$false; liveMentoring=$false; learnerMastery=$false; skipped=@(); runs=@()}
$oldGit = @{}
Get-ChildItem Env: | Where-Object Name -Like 'GIT_*' | ForEach-Object { $oldGit[$_.Name]=$_.Value; Remove-Item ('Env:'+ $_.Name) }
$env:GIT_CONFIG_NOSYSTEM='1'
$env:GIT_CONFIG_GLOBAL=Join-Path $stage 'empty-git-config'
[IO.File]::WriteAllText($env:GIT_CONFIG_GLOBAL,'')
try {
    $inputs = @('.gitignore','course/university/pilot.puc','course/university/missions/u1-boundaries.pum','course/university/missions/003-object-types.pum','src/main.cpp','src/UI/AppUISetup.h','src/UI/AppUISetup.cpp','visualstudio/Pandata.bank.vcxproj')
    $inputs += @(Get-ChildItem -LiteralPath $root -Filter '*.cmd' | ForEach-Object Name)
    $inputs += @(Get-ChildItem -File -Recurse -LiteralPath (Join-Path $root 'university') | Where-Object Extension -In '.cpp','.h','.py','.ps1','.md','.in','.json' | ForEach-Object { $_.FullName.Substring($root.Length+1) })
    foreach ($relative in $inputs) {
        $src=Join-Path $root $relative; NoReparse $src
        $dest=Join-Path $copy $relative
        $null=New-Item -ItemType Directory -Force -Path (Split-Path $dest)
        Copy-Item -LiteralPath $src -Destination $dest
    }
    $workflow = Get-Content -Raw -LiteralPath (Join-Path $copy 'university/tests/offline_workflow.json') | ConvertFrom-Json
    $template=Join-Path $stage 'empty-git-template'; $null=New-Item -ItemType Directory -Path $template
    $null=InvokeTest 'disposable-git-init' $git ('init --template="'+$template+'"') $copy
    [IO.File]::WriteAllText((Join-Path $copy 'validation-diff.txt'),"before`n")
    $null=InvokeTest 'disposable-git-add' $git 'add .' $copy
    $null=InvokeTest 'disposable-git-commit' $git '-c user.name=Validation -c user.email=validation@example.invalid -c commit.gpgsign=false commit -m synthetic-baseline' $copy
    [IO.File]::AppendAllText((Join-Path $copy 'validation-diff.txt'),"after-for-read-only-diff`n")
    $copyBefore=Map $copy @('.git','src','validation-diff.txt')

    $logs=Batch 'retained-regressions' (Join-Path $copy 'Validate-Offline-Foundation.cmd')
    Need ($logs.Contains('PASS: combined offline foundation;')) 'Retained regressions lacked final PASS'
    $logs -split "`r?`n" | Where-Object { $_ -match '^(PASS:|SKIP:)' } | ForEach-Object { Write-Host $_ }
    if ($logs.Contains('SKIP: symlink creation denied; not passed.')) { $receipt.skipped += 'Windows symlink creation denied; not passed' }
    $logs=Batch 'launcher-first' (Join-Path $copy 'Start-Pandata-University.cmd') '--no-pause' (InputFile 'first')
    Need ($logs.Contains('003-object-types') -and $logs.Contains('Offline Foundation 1.0')) 'Actual launcher/lesson display absent'
    $null=Batch 'build-final-check' (Join-Path $copy 'Build-Offline-Acceptance.cmd') ('"'+$stage+'"')
    $check=Join-Path $stage 'final-state.exe'
    $null=InvokeTest 'first-saved-state' $check ('--first "'+$copy+'"') $stage
    $logs=Batch 'launcher-restart' (Join-Path $copy 'Start-Pandata-University.cmd') '--no-pause' (InputFile 'restart')
    Need ($logs.Contains('SYNTHETIC final prediction')) 'Saved prediction not displayed after restart'
    $null=InvokeTest 'complete-saved-state' $check ('--complete "'+$copy+'"') $stage
    $launches=@(Get-ChildItem -Directory -LiteralPath (Join-Path $copy 'build/university') -Filter 'launch-*')
    Need ($launches.Count -eq 2) 'Expected two clean actual launcher builds'
    $app=Join-Path $launches[0].FullName 'pandata-university.exe'
    $progress=Map $copy @('course/university/progress')
    $logs=InvokeTest 'refused-commands' $app '' $copy (InputFile 'refused')
    Need ($logs.Contains('REJECTED:') -and $logs.Contains('Action/step not completed:')) 'Missing refusal diagnostics'
    Need (!$logs.Contains('DISPLAYED:') -and !$logs.Contains('Action succeeded')) 'Malformed input activated a capability'
    Need (!(Test-Path -LiteralPath (Join-Path $copy 'course/university/evidence'))) 'Malformed input created action/mentor evidence'
    Same $progress (Map $copy @('course/university/progress')) 'Malformed commands changed progress'
    Write-Host 'EXPECTED REFUSAL: malformed/traversal/apply/write commands; interactive session exits 0'
    $null=Batch 'launcher-extra-argument' (Join-Path $copy 'Start-Pandata-University.cmd') '--no-pause extra' '' 2
    $logs=InvokeTest 'read-only-actions' $app '' $copy (InputFile 'views')
    Need ($logs.Contains('src/main.cpp (read-only)') -and $logs.Contains('after-for-read-only-diff')) 'Actual file/Git diff not displayed'
    $evidence=@(Get-ChildItem -Recurse -File -LiteralPath (Join-Path $copy 'course/university/evidence/u2') -Filter 'evidence.json' | ForEach-Object { Get-Content -Raw $_.FullName | ConvertFrom-Json })
    foreach ($action in @('git-status','git-diff')) {
        $found=@($evidence | Where-Object action -EQ $action)
        Need ($found.Count -eq 1 -and $found[0].passed) "Missing successful machine evidence for $action"
    }
    Same $copyBefore (Map $copy @('.git','src','validation-diff.txt')) 'Read-only actions changed disposable Git/source'
    Same $progress (Map $copy @('course/university/progress')) 'Read-only actions changed progress'

    $backup=Join-Path $stage 'verified-backup'; $null=New-Item -ItemType Directory -Path (Join-Path $backup 'course/university')
    foreach ($part in @('missions','progress','pilot.puc')) { Copy-Item -Recurse -LiteralPath (Join-Path $copy "course/university/$part") -Destination (Join-Path $backup 'course/university') }
    Same (Map $copy @('course/university/missions','course/university/progress','course/university/pilot.puc')) (Map $backup @('course/university/missions','course/university/progress','course/university/pilot.puc')) 'Synthetic backup mismatch'
    $null=InvokeTest 'backup-readable' $check ('--complete "'+$backup+'"') $stage
    $saved=Join-Path $copy 'course/university/progress/003-object-types'
    $next=1+[int]((Get-ChildItem -LiteralPath $saved -Filter '*.pup' | ForEach-Object { [int]$_.BaseName } | Measure-Object -Maximum).Maximum)
    $corrupt=Join-Path $saved ($next.ToString()+'.pup'); [IO.File]::WriteAllText($corrupt,"deliberate corrupt synthetic snapshot`n")
    $logs=InvokeTest 'recovery-session' $app '' $copy (InputFile 'report')
    Need ($logs.Contains('Recovery:') -and (Test-Path $corrupt)) 'Corruption was not preserved/reported'
    $null=InvokeTest 'recovery-state' $check ('--recovered "'+$copy+'"') $stage
    $definition=Join-Path $copy 'course/university/missions/003-object-types.pum'; $original=[IO.File]::ReadAllBytes($definition)
    [IO.File]::AppendAllText($definition,"`n# deliberate synthetic mismatch`n")
    $mismatchProgress=Map $copy @('course/university/progress')
    $logs=InvokeTest 'mismatch-session' $app '' $copy (InputFile 'blocked')
    Need ($logs.Contains('Action/step not completed:')) 'Definition mismatch was not visibly refused'
    $null=InvokeTest 'mismatch-state' $check ('--blocked "'+$copy+'"') $stage
    Same $mismatchProgress (Map $copy @('course/university/progress')) 'Mismatch changed saved progress'
    Write-Host 'EXPECTED REFUSAL: mismatched definition; no migration/reset'
    [IO.File]::WriteAllBytes($definition,$original)
    Remove-Item -LiteralPath $corrupt # Only the exact synthetic bad snapshot created above.
    $null=InvokeTest 'fresh-session' $app '' $copy (InputFile 'fresh')
    $null=InvokeTest 'fresh-state' $check ('--complete "'+$copy+'"') $stage
    Same $copyBefore (Map $copy @('.git','src','validation-diff.txt')) 'Workflow changed disposable Git/source'
    Same $before (Map $root $protected) 'Real protected files changed'
    $receipt['protectedDigest']=Digest $before
    $receipt['sourceDigest']=Digest (Map $root @('university','Start-Pandata-University.cmd','Build-University.cmd'))
    $receipt.passed=$true
} catch {
    $receipt['error']=$_.Exception.Message; Write-Host ('FAIL: '+$_.Exception.Message)
} finally {
    try { Same $before (Map $root $protected) 'Real protected files changed'; Write-Host 'PASS: real learner state, PandaBank source and Git history unchanged' }
    catch { $receipt.passed=$false; $receipt['preservationError']=$_.Exception.Message; Write-Host ('FAIL: '+$_.Exception.Message) }
    $receipt | ConvertTo-Json -Depth 8 | Set-Content -Encoding UTF8 -LiteralPath (Join-Path $stage 'acceptance.json')
    Get-ChildItem Env: | Where-Object Name -Like 'GIT_*' | ForEach-Object { Remove-Item ('Env:'+ $_.Name) }
    foreach ($key in $oldGit.Keys) { Set-Item ('Env:'+$key) $oldGit[$key] }
    Write-Host "Acceptance receipt: $stage\acceptance.json"
}
if (!$receipt.passed) { exit 1 }
Write-Host 'PASS: PU-OFFLINE-1.0 launch-ready offline baseline; Windows run 2 of 2 complete.'
exit 0
