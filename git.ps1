$ErrorActionPreference = "Stop"

# Dynamically get the script folder and point to git.cpp
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$filePath = Join-Path $scriptDir "git.cpp"

# Log the file path being used
"[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] Using file path: $filePath" | Out-File -Append "D:\Logs\git-task-output.txt"

# Check if file exists
if (!(Test-Path $filePath)) {
    "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] File not found: $filePath" | Out-File -Append "D:\Logs\git-task-output.txt"
    exit 1
}

try {
    # Read all lines, or initialize an empty list if the file is empty
    $lines = Get-Content $filePath -ErrorAction SilentlyContinue
    if (-not $lines) {
        $lines = @()  # Initialize empty array if file is empty
    }

    # Replace or add first line with today's date
    $dateString = (Get-Date).ToString("yyyy-MM-dd HH:mm:ss")
    if ($lines.Count -ge 1) {
        $lines[0] = "//" + $dateString
    } else {
        $lines += "//" + $dateString
    }

    # Write updated lines back to the file
    $lines | Set-Content $filePath

    # Git operations
    Push-Location (Split-Path $filePath)
    git add $filePath
    git commit -m "Auto Update $dateString"
    git push
    Pop-Location

    "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] Successfully Auto-Updated Git: $filePath" | Out-File -Append "D:\Logs\git-task-output.txt"

}
catch {
    Write-Host "An error occurred: $_" 
    "[$(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')] An Error Occurred: $filePath" | Out-File -Append "D:\Logs\git-task-output.txt"
    exit 1
}
