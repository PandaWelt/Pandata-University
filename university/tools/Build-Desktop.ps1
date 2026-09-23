param([Parameter(Mandatory=$true)][string]$Repository, [Parameter(Mandatory=$true)][string]$Output, [switch]$Tests)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
try {
    $root=(Resolve-Path -LiteralPath $Repository).Path
    $cl=(Get-Command cl.exe -CommandType Application -ErrorAction Stop).Source
    $out=[IO.Path]::GetFullPath($Output)
    $null=New-Item -ItemType Directory -Force -Path $out
    $list=Get-Content -Raw -LiteralPath (Join-Path $root 'university/desktop-sources.json') | ConvertFrom-Json
    $common=@($list.core)+@($list.view)+@($list.imgui)
    function Compile([string]$name,[string[]]$sources,[bool]$window) {
        if (@($sources | Select-Object -Unique).Count -ne $sources.Count) { throw 'Duplicate build source.' }
        $lines=@('/nologo','/std:c++20','/EHsc','/W4','/utf-8')
        foreach($inc in @('university/include','thirdparty/imgui','thirdparty/imgui/backends','thirdparty/SDL2/include')) { $lines += '/I"'+(Join-Path $root $inc)+'"' }
        foreach($relative in $sources) {
            if($relative -notmatch '^(university|thirdparty)/[A-Za-z0-9_./-]+\.cpp$' -or $relative.Contains('..')) { throw 'Unsafe build source.' }
            $path=Join-Path $root $relative
            if(!(Test-Path -LiteralPath $path -PathType Leaf)) { throw "Missing source: $relative" }
            $lines += '"'+$path+'"'
        }
        $lines += '/Fe:"'+(Join-Path $out $name)+'"'
        if($window) {
            $lines += @('/link','/SUBSYSTEM:WINDOWS','/ENTRY:wmainCRTStartup','opengl32.lib')
            $lines += '"'+(Join-Path $root 'thirdparty/SDL2/lib/x64/SDL2.lib')+'"'
        }
        $response=Join-Path $out ($name+'.rsp')
        [IO.File]::WriteAllLines($response,$lines,[Text.Encoding]::Unicode)
        Push-Location -LiteralPath $out
        try { & $cl ('@'+$response); if($LASTEXITCODE -ne 0) { throw "C++ build failed: $name" } }
        finally { Pop-Location }
    }
    Compile 'pandata-university-desktop.exe' ($common+@($list.host)+@($list.backends)) $true
    Copy-Item -LiteralPath (Join-Path $root 'thirdparty/SDL2/lib/x64/SDL2.dll') -Destination $out
    Copy-Item -LiteralPath (Join-Path $root 'university/licenses/DESKTOP_NOTICES.txt') -Destination $out
    if($Tests) { Compile 'desktop-tests.exe' ($common+@($list.tests)) $false }
    Write-Host "PASS: desktop build and runtime assets: $out"
    exit 0
} catch { Write-Host ('FAIL: '+$_.Exception.Message); Write-Host 'Use x64 Developer PowerShell for VS 2022. Existing console/progress were not changed.'; exit 1 }
