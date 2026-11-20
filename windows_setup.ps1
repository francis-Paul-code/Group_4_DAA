# setup-windows.ps1
param(
    # Override this if MSYS2 is already installed elsewhere.
    [string]$MsysRoot = 'C:\msys64'
)

$ErrorActionPreference = 'Stop'
$bash = Join-Path $MsysRoot 'usr\bin\bash.exe'

if (-not (Test-Path -LiteralPath $bash -PathType Leaf)) {
    if ($MsysRoot -ne 'C:\msys64') {
        throw "MSYS2 was not found at '$MsysRoot'. Supply its existing installation directory or use the default."
    }
    if (-not (Get-Command winget.exe -ErrorAction SilentlyContinue)) {
        throw 'Install Windows App Installer to get winget, or install MSYS2 and rerun this script.'
    }

    Write-Host 'Installing MSYS2...'
    winget.exe install --id MSYS2.MSYS2 --exact --source winget --silent `
        --accept-package-agreements --accept-source-agreements

    if ($LASTEXITCODE -ne 0) {
        throw "MSYS2 installation failed (winget exit code $LASTEXITCODE)."
    }
    if (-not (Test-Path -LiteralPath $bash -PathType Leaf)) {
        throw "Cannot find '$bash'. If MSYS2 is installed elsewhere, rerun with -MsysRoot."
    }
}

function Invoke-Msys([string]$Command) {
    & $bash -lc $Command
    if ($LASTEXITCODE -ne 0) {
        throw "MSYS2 command failed (exit code $LASTEXITCODE): $Command"
    }
}

$previousSystem = $env:MSYSTEM
try {
    $env:MSYSTEM = 'UCRT64'

    Write-Host 'Updating MSYS2...'
    # A core update can close the first shell. Finish in a fresh shell.
    & $bash -lc 'pacman -Syu --noconfirm'
    if ($LASTEXITCODE -ne 0) {
        Write-Host 'First update pass was interrupted. Retrying in a fresh shell...'
    }
    Invoke-Msys 'pacman -Syu --noconfirm'

    Write-Host 'Installing GCC, CMake, Ninja, pkg-config, and GTK4...'
    $packages = 'gcc', 'cmake', 'ninja', 'pkgconf', 'gtk4' |
        ForEach-Object { "mingw-w64-ucrt-x86_64-$_" }
    Invoke-Msys "pacman -S --needed --noconfirm $($packages -join ' ')"

    Write-Host 'Checking the build tools and GTK4...'
    Invoke-Msys 'gcc --version && g++ --version && cmake --version && ninja --version && pkg-config --cflags --libs gtk4'
}
finally {
    $env:MSYSTEM = $previousSystem
}

Write-Host 'Setup complete. Open MSYS2 UCRT64 in the project directory and run:'
Write-Host '  cmake -S . -B build-ucrt64 -G Ninja'
Write-Host '  cmake --build build-ucrt64'