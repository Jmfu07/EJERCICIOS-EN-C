param(
    [switch]$Interactivo
)

$ErrorActionPreference = 'SilentlyContinue'
$vscodeDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Split-Path -Parent $vscodeDir

function Find-FirstExisting([string[]]$candidates) {
    foreach ($p in $candidates) {
        if ($p -and (Test-Path -LiteralPath $p)) { return (Resolve-Path -LiteralPath $p).Path }
    }
    return $null
}

$pathGccs = @(& where.exe gcc 2>$null)
$pathGdbs = @(& where.exe gdb 2>$null)

$msysGcc = Find-FirstExisting @(
    "$root\tools\msys2\ucrt64\bin\gcc.exe",
    "$root\tools\msys2\mingw64\bin\gcc.exe",
    "C:\LENGUAJES\MSYS2\ucrt64\bin\gcc.exe",
    "C:\LENGUAJES\MSYS2\mingw64\bin\gcc.exe",
    "C:\msys64\ucrt64\bin\gcc.exe",
    "C:\msys64\mingw64\bin\gcc.exe",
    "D:\msys64\ucrt64\bin\gcc.exe",
    ($pathGccs | Where-Object { $_ -match 'msys|ucrt64|mingw64' } | Select-Object -First 1)
)

$tdmGcc = Find-FirstExisting @(
    "$root\tools\tdm-gcc\bin\gcc.exe",
    "C:\LENGUAJES\TDM-GCC\bin\gcc.exe",
    "C:\TDM-GCC-64\bin\gcc.exe",
    "C:\Program Files (x86)\Dev-Cpp\TDM-GCC-64\bin\gcc.exe",
    "C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe",
    "C:\Dev-Cpp\TDM-GCC-64\bin\gcc.exe",
    ($pathGccs | Where-Object { $_ -match 'tdm|dev-cpp' } | Select-Object -First 1)
)

$anyGcc = if ($msysGcc) { $msysGcc } elseif ($tdmGcc) { $tdmGcc } else { Find-FirstExisting $pathGccs }

function Get-SiblingGdb([string]$gccPath) {
    if (-not $gccPath) { return $null }
    $cand = Join-Path (Split-Path -Parent $gccPath) "gdb.exe"
    if (Test-Path -LiteralPath $cand) { return $cand }
    return $null
}

$msysGdb = Get-SiblingGdb $msysGcc
$tdmGdb  = Get-SiblingGdb $tdmGcc
$anyGdb  = if ($msysGdb) { $msysGdb } elseif ($tdmGdb) { $tdmGdb } else { Find-FirstExisting $pathGdbs }

Write-Host "=== Auto-configuracion de entorno C (VS Code) ==="
Write-Host "  MSYS2   : $(if ($msysGcc) { $msysGcc } else { 'No encontrado' })"
Write-Host "  TDM-GCC : $(if ($tdmGcc)  { $tdmGcc }  else { 'No encontrado' })"
Write-Host "  GDB     : $(if ($anyGdb)  { $anyGdb }  else { 'No encontrado (usa tarea Sin GDB)' })"

if (-not $anyGcc) {
    Write-Host ""
    Write-Host "[AVISO] No se encontro ningun compilador GCC en esta PC." -ForegroundColor Yellow
    if ($Interactivo) {
        $resp = Read-Host "Deseas instalar MSYS2 automaticamente con winget? (S/N)"
        if ($resp -match '^[sS]$') {
            winget install -e --id MSYS2.MSYS2
        }
    }
    exit 0
}

# 1. Actualizar .vscode/settings.json con el compilador detectado para IntelliSense
$settingsPath = Join-Path $vscodeDir "settings.json"
$settings = [ordered]@{
    "C_Cpp.default.compilerPath"     = $anyGcc
    "C_Cpp.default.cStandard"        = "c11"
    "C_Cpp.default.intelliSenseMode" = "windows-gcc-x64"
    "files.encoding"                 = "utf8"
}
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)
$settingsJson = ($settings | ConvertTo-Json -Depth 4)
[System.IO.File]::WriteAllText($settingsPath, $settingsJson, $utf8NoBom)

# 2. Actualizar miDebuggerPath en .vscode/launch.json segun lo detectado en esta PC
$launchPath = Join-Path $vscodeDir "launch.json"
if (Test-Path -LiteralPath $launchPath) {
    $launch = Get-Content -LiteralPath $launchPath -Raw | ConvertFrom-Json
    foreach ($cfg in $launch.configurations) {
        if ($cfg.name -match 'MSYS2' -and $msysGdb) {
            $cfg.miDebuggerPath = $msysGdb
        } elseif ($cfg.name -match 'TDM' -and $tdmGdb) {
            $cfg.miDebuggerPath = $tdmGdb
        } elseif ($anyGdb) {
            $cfg.miDebuggerPath = $anyGdb
        } else {
            $cfg.miDebuggerPath = "gdb.exe"
        }
    }
    $launchJson = ($launch | ConvertTo-Json -Depth 8)
    [System.IO.File]::WriteAllText($launchPath, $launchJson, $utf8NoBom)
}

Write-Host "[OK] Entorno configurado automaticamente. Pulsa F5 en cualquier archivo .c." -ForegroundColor Green
