# Setup script to add 'cTodo' command to PowerShell

# Get the path to the executable
$todoExePath = Split-Path -Parent $PSCommandPath
$todoExePath = Join-Path $todoExePath "build\todo-cli.exe"

if (-not (Test-Path $todoExePath)) {
    Write-Host "Error: todo-cli.exe not found at $todoExePath" -ForegroundColor Red
    Write-Host "Please build the project first using:" -ForegroundColor Yellow
    Write-Host "  cd c-impl" -ForegroundColor Yellow
    Write-Host "  build-gcc.bat" -ForegroundColor Yellow
    exit 1
}

# Get the PowerShell profile path
$profilePath = $PROFILE.CurrentUserCurrentHost

# Create profile if it doesn't exist
if (-not (Test-Path $profilePath)) {
    Write-Host "Creating PowerShell profile at: $profilePath" -ForegroundColor Green
    $profileDir = Split-Path -Parent $profilePath
    if (-not (Test-Path $profileDir)) {
        New-Item -ItemType Directory -Path $profileDir -Force | Out-Null
    }
    New-Item -ItemType File -Path $profilePath -Force | Out-Null
}

# Create the alias command text for profile
$aliasCommandText = "Set-Alias -Name cTodo -Value '$todoExePath' -Scope Global -Force"

# Check if alias already exists in profile
$profileContent = Get-Content $profilePath -Raw -ErrorAction SilentlyContinue

if ($profileContent -like "*cTodo*") {
    Write-Host "cTodo alias already configured in profile" -ForegroundColor Yellow
} else {
    Write-Host "Adding cTodo alias to PowerShell profile..." -ForegroundColor Green
    Add-Content -Path $profilePath -Value "`n# Todo CLI Command`n$aliasCommandText`n" -Encoding UTF8
    Write-Host "cTodo alias added to: $profilePath" -ForegroundColor Green
}

# Apply the alias to current session directly (not as string)
Set-Alias -Name cTodo -Value $todoExePath -Scope Global -Force

Write-Host ""
Write-Host "Setup complete!" -ForegroundColor Green
Write-Host ""
Write-Host "You can now use 'cTodo' command from anywhere in PowerShell" -ForegroundColor Cyan
Write-Host ""
Write-Host "Usage:"
Write-Host "  cTodo          - Opens the todo application" -ForegroundColor Yellow
Write-Host ""
Write-Host "Note: The profile will be loaded automatically in new PowerShell windows" -ForegroundColor Cyan
