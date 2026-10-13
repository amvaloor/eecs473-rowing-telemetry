# This script is courtesy of Gemini
$ErrorActionPreference = "Stop"

# Resolve project root relative to script location
$ProjectRoot = Resolve-Path "$PSScriptRoot\.."
$TargetDir = "$ProjectRoot\firmware\components"
$ArduinoLibDir = "$env:USERPROFILE\Documents\Arduino\libraries"

# Ensure Arduino libraries folder exists
if (!(Test-Path $ArduinoLibDir)) {
    New-Item -ItemType Directory -Path $ArduinoLibDir | Out-Null
}

# Create single symlink pointing to components folder or individual subfolders
New-Item -ItemType SymbolicLink -Path "$ArduinoLibDir\rowing_components" -Target $TargetDir -Force

Write-Host "Arduino library symlinks created successfully!" -ForegroundColor Green
