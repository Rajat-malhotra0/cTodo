# Windows Build Script for cTodo
# Compiles the C application and sets up global 'cTodo' command

param(
    [switch]$SkipSetup = $false
)

# Colors for output
$green = "`e[32m"
$red = "`e[31m"
$yellow = "`e[33m"
$reset = "`e[0m"

Write-Host "${green}================================================== `n            cTodo - Build for Windows `n==================================================${reset}`n"

# Check if cJSON exists
if (-not (Test-Path "cJSON")) {
    Write-Host "Downloading cJSON library..."
    git clone https://github.com/DaveGamble/cJSON.git
    if ($LASTEXITCODE -ne 0) {
        Write-Host "${red}Error: Failed to clone cJSON${reset}"
        exit 1
    }
}

# Compile cJSON if not already compiled
if (-not (Test-Path "cJSON\cJSON.o")) {
    Write-Host "Compiling cJSON..."
    Push-Location cJSON
    gcc -c -std=c99 cJSON.c -o cJSON.o
    if ($LASTEXITCODE -ne 0) {
        Write-Host "${red}Error: Failed to compile cJSON. Make sure GCC is installed and in PATH${reset}"
        Pop-Location
        exit 1
    }
    Pop-Location
}

# Create build directory
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

# Compile the application
Write-Host "Compiling cTodo..."
$buildDir = (Resolve-Path "build").Path
$exePath = "$buildDir\todo-cli.exe"

gcc -std=c99 -Wall -Wextra -I.\include -I.\cJSON `
    -o $exePath `
    src\main.c src\todo.c src\storage.c src\ui.c `
    cJSON\cJSON.o

if ($LASTEXITCODE -ne 0) {
    Write-Host "`n${red}Error: Compilation failed${reset}"
    exit 1
}

Write-Host "`n${green}================================================== `n            Build Successful `n==================================================${reset}"
Write-Host "Executable created at: $exePath`n"

if ($SkipSetup) {
    Write-Host "Skipping setup. Run this script without -SkipSetup to set up global command."
    exit 0
}

# Setup global command
Write-Host "${yellow}Setting up 'cTodo' as a global command...${reset}`n"

# Ensure profile directory exists
$profileDir = Split-Path -Parent $PROFILE.CurrentUserCurrentHost
if (-not (Test-Path $profileDir)) {
    New-Item -ItemType Directory -Path $profileDir -Force | Out-Null
}

# Add alias to PowerShell profile
$aliasCommand = "`nSet-Alias -Name ctodo -Value '$exePath' -Scope Global -Force"
Add-Content -Path $PROFILE.CurrentUserCurrentHost -Value $aliasCommand -Encoding UTF8

Write-Host "${green}Setup complete!${reset}"
Write-Host "Close and reopen PowerShell, then type: ${yellow}ctodo${reset}"
Write-Host "`nNote: If the command doesn't work immediately, restart PowerShell or run: . $PROFILE"
