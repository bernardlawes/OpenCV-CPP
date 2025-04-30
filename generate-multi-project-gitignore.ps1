$gitignoreContent = @"
# === OS-Specific ===
.DS_Store
Thumbs.db

# === Build Artifacts ===
build/
install/
bin/
lib/
*.exe
*.dll
*.so
*.dylib
*.a
*.o
*.obj
*.pch
*.ilk
*.pdb
*.idb

# === CMake ===
CMakeFiles/
CMakeCache.txt
cmake_install.cmake
Makefile

# === VS Code ===
.vscode/
*.code-workspace

# === Logs and Generated Files ===
*.log
*.tmp
*.swp
*.swo

# === Compiler-generated ===
*.out
*.gch

# === Input folders (test/intermediate data) ===
input/

# === Backup files ===
*~
*.bak

# === Vcpkg build data ===
.vcpkg-root/
vcpkg_installed/

# === Misc ===
*.user
"@

$gitignorePath = ".gitignore"

if (Test-Path $gitignorePath) {
    Write-Host ".gitignore already exists. Overwrite? [y/N]: " -NoNewline
    $response = Read-Host
    if ($response -ne 'y' -and $response -ne 'Y') {
        Write-Host "Aborting without overwriting existing .gitignore."
        exit 0
    }
}

$gitignoreContent | Set-Content $gitignorePath -Encoding UTF8
Write-Host ".gitignore has been created successfully at the repo root."
