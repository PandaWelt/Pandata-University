param([Parameter(Mandatory=$true)][string]$Repository)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
function Need([bool]$ok,[string]$why) { if(!$ok) { throw $why } }
function SafePath([string]$path) {
    for($p=[IO.Path]::GetFullPath($path);$p;$p=[IO.Path]::GetDirectoryName($p)) {
        if(Test-Path -LiteralPath $p) {
            Need (((Get-Item -Force -LiteralPath $p).Attributes -band [IO.FileAttributes]::ReparsePoint) -eq 0) "Reparse path refused: $p"
        }
    }
}
function Hashes([string]$root,[string[]]$names) {
    $map=@{}
    foreach($name in $names) {
        $p=Join-Path $root $name; SafePath $p
        if(!(Test-Path -LiteralPath $p)) { continue }
        $items=@(Get-Item -Force -LiteralPath $p)
        if($items[0].PSIsContainer) { $items=@(Get-ChildItem -Recurse -Force -LiteralPath $p) }
        foreach($item in $items) {
            SafePath $item.FullName
            if(!$item.PSIsContainer) { $map[$item.FullName.Substring($root.Length)]=(Get-FileHash -Algorithm SHA256 -LiteralPath $item.FullName).Hash }
        }
    }
    return $map
}
function Unchanged($a,$b) {
    Need ($a.Count -eq $b.Count) 'Protected file count changed.'
    foreach($key in $a.Keys) { Need ($b.ContainsKey($key) -and $a[$key] -eq $b[$key]) "Protected file changed: $key" }
}
function Run([string]$name,[string]$exe,[string]$arguments,[int]$expected=0) {
    $stdout=Join-Path $stage ($name+'.stdout.txt'); $stderr=Join-Path $stage ($name+'.stderr.txt')
    $p=Start-Process -FilePath $exe -ArgumentList $arguments -WorkingDirectory $stage -NoNewWindow -PassThru -RedirectStandardOutput $stdout -RedirectStandardError $stderr
    $null=$p.Handle; $clock=[Diagnostics.Stopwatch]::StartNew()
    while(!$p.WaitForExit(500)) {
        if($clock.Elapsed.TotalSeconds -gt 600 -or ((Get-Item $stdout).Length+(Get-Item $stderr).Length) -gt 8MB) {
            & "$env:SystemRoot\System32\taskkill.exe" /PID $p.Id /T /F | Out-Null
            throw "Process bound exceeded: $name"
        }
    }
    $p.WaitForExit()
    Need (((Get-Item $stdout).Length+(Get-Item $stderr).Length) -le 8MB) "Output bound exceeded: $name"
    Need ($null -ne $p.ExitCode) "No exit status: $name"
    $receipt.runs += [ordered]@{name=$name;exit=$p.ExitCode;expected=$expected;executable=$exe;arguments=$arguments;stdout=$stdout;stderr=$stderr}
    Need ($p.ExitCode -eq $expected) "$name exited $($p.ExitCode), expected $expected. Inspect $stdout and $stderr"
    if($expected) { Write-Host "EXPECTED REFUSAL: $name (exit $expected)" } else { Write-Host "PASS: $name" }
    return [IO.File]::ReadAllText($stdout)+[IO.File]::ReadAllText($stderr)
}
$root=(Resolve-Path -LiteralPath $Repository).Path.TrimEnd('\','/')
SafePath $root
$null=Get-Command cl.exe -ErrorAction Stop
# Short physical staging, no nested checkout inside the repository and no SUBST workaround.
$temp=[IO.Path]::GetTempPath().TrimEnd('\','/'); SafePath $temp
$stage=Join-Path $temp ('pug-'+[guid]::NewGuid().ToString('N').Substring(0,8))
Need ($stage.Length -lt 100) 'Temporary path is too long. Set TEMP to a short physical writable directory, then run this validator.'
Need (!(Test-Path -LiteralPath $stage)) 'Stage already exists; nothing replaced.'
$null=New-Item -ItemType Directory -Path $stage
Write-Host "Desktop milestone validation staging: $stage"
$protected=@('src','thirdparty','.git','course','university','Start-Pandata-University.cmd','Build-University.cmd')
$before=Hashes $root $protected
$receipt=[ordered]@{milestone='PU-DESKTOP-1';passed=$false;foundation='accepted c4f6640; not reopened';liveMentoring=$false;foundationSuitesRerun=$false;runs=@();skipped=@()}
$oldGit=@{}
Get-ChildItem Env: | Where-Object Name -Like 'GIT_*' | ForEach-Object { $oldGit[$_.Name]=$_.Value; Remove-Item ('Env:'+$_.Name) }
$env:GIT_CONFIG_NOSYSTEM='1'; $env:GIT_CONFIG_GLOBAL=Join-Path $stage 'empty-git-config'
[IO.File]::WriteAllText($env:GIT_CONFIG_GLOBAL,'')
try {
    $build=Join-Path $root 'university/tools/Build-Desktop.ps1'
    $logs=Run 'build' "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe" ('-NoLogo -NoProfile -ExecutionPolicy Bypass -File "'+$build+'" -Repository "'+$root+'" -Output "'+$stage+'" -Tests')
    Need ($logs.Contains('PASS: desktop build')) 'Build did not finish with runtime assets.'
    # C++ checks seed their own new state and never open the real progress directory.
    $sandbox=Join-Path $stage 'study'
    $logs=Run 'controller-render' (Join-Path $stage 'desktop-tests.exe') ('"'+$root+'" "'+$sandbox+'"')
    Need ($logs.Contains('desktop controller/render checks')) 'Missing desktop checks result.'
    $logs -split "`r?`n" | Where-Object { $_ -match '^PASS:' } | ForEach-Object { Write-Host $_ }
    $smoke=Join-Path $stage 'smoke'
    foreach($relative in @('course/university/missions/u1-boundaries.pum','course/university/missions/003-object-types.pum','course/university/pilot.puc','src/main.cpp','university/include/Mission.h')) {
        $dest=Join-Path $smoke $relative; $null=New-Item -ItemType Directory -Force -Path (Split-Path $dest)
        Copy-Item -LiteralPath (Join-Path $root $relative) -Destination $dest
    }
    $smokeBefore=Hashes $smoke @('course','src','university')
    $logs=Run 'graphics-smoke' (Join-Path $stage 'pandata-university-desktop.exe') ('--repository "'+$smoke+'" --smoke')
    Need ($logs.Contains('PASS: desktop graphics smoke; 20 frames')) 'No actual graphics rendering proof.'
    $null=Run 'invalid-argument' (Join-Path $stage 'pandata-university-desktop.exe') ('--repository "'+$smoke+'" --smoke --apply') 1
    $null=Run 'fresh-graphics' (Join-Path $stage 'pandata-university-desktop.exe') ('--repository "'+$smoke+'" --smoke')
    Unchanged $smokeBefore (Hashes $smoke @('course','src','university'))
    Need (!(Test-Path -LiteralPath (Join-Path $smoke 'course/university/progress'))) 'Graphics smoke wrote progress.'
    Unchanged $before (Hashes $root $protected)
    # Install only this explicitly requested, built-and-checked desktop artifact.
    $install=Join-Path $root 'build/university/desktop'; SafePath $install
    $null=New-Item -ItemType Directory -Force -Path $install
    foreach($name in @('pandata-university-desktop.exe','SDL2.dll','DESKTOP_NOTICES.txt')) {
        $dest=Join-Path $install $name; SafePath $dest
        Copy-Item -LiteralPath (Join-Path $stage $name) -Destination $dest -Force
        Need ((Get-FileHash -LiteralPath $dest).Hash -eq (Get-FileHash -LiteralPath (Join-Path $stage $name)).Hash) 'Installed artifact hash mismatch.'
    }
    $receipt['installedExecutable']=Join-Path $install 'pandata-university-desktop.exe'
    $receipt['executableSha256']=(Get-FileHash -LiteralPath $receipt.installedExecutable).Hash
    $receipt.passed=$true
} catch { $receipt['error']=$_.Exception.Message; Write-Host ('FAIL: '+$_.Exception.Message) }
finally {
    try { Unchanged $before (Hashes $root $protected); Write-Host 'PASS: PandaBank, console, definitions, real progress and Git bytes preserved' }
    catch { $receipt.passed=$false; $receipt['preservationError']=$_.Exception.Message; Write-Host ('FAIL: '+$_.Exception.Message) }
    $receipt | ConvertTo-Json -Depth 8 | Set-Content -Encoding UTF8 -LiteralPath (Join-Path $stage 'desktop-acceptance.json')
    Get-ChildItem Env: | Where-Object Name -Like 'GIT_*' | ForEach-Object { Remove-Item ('Env:'+$_.Name) }
    foreach($key in $oldGit.Keys) { Set-Item ('Env:'+$key) $oldGit[$key] }
    Write-Host "Receipt and logs: $stage"
}
if(!$receipt.passed) { exit 1 }
Write-Host 'PASS: PU-DESKTOP-1 build, controller/render checks, graphics/refusal/fresh and preservation.'
Write-Host 'Next: launch Start-Pandata-University-Desktop.cmd and perform the short visual inspection in docs/DESKTOP_MILESTONE.md.'
exit 0
