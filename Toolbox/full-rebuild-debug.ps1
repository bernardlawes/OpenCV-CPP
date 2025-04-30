# Full Clean Rebuild Script for Bernard's C++ OpenCV Projects

# ---- Settings ----
$projectRoot = $PSScriptRoot
$buildDir = "$projectRoot/build"
$installDir = "$projectRoot/install"
$vcpkgToolchain = "D:/Library/vcpkg/scripts/buildsystems/vcpkg.cmake"
$buildType = "Debug"  # Change to "Release" if needed

# Resolve vcpkg binary folder
if ($buildType -eq "Debug") {
    $vcpkgBinDir = "D:/Library/vcpkg/installed/x64-windows/debug/bin"
    $suffix = "d"
} else {
    $vcpkgBinDir = "D:/Library/vcpkg/installed/x64-windows/bin"
    $suffix = ""
}

# ---- Start Script ----
Write-Host "`n=== Cleaning Build Directory ==="
if (Test-Path $buildDir) {
    Remove-Item "$buildDir/*" -Recurse -Force
} else {
    New-Item -ItemType Directory -Path $buildDir
}

Write-Host "`n=== Cleaning Install Directory ==="
if (Test-Path $installDir) {
    Remove-Item "$installDir/*" -Recurse -Force
} else {
    New-Item -ItemType Directory -Path $installDir
}

# Move into build folder
Set-Location $buildDir

Write-Host "`n=== Configuring CMake for $buildType ==="
cmake .. -DCMAKE_INSTALL_PREFIX="$installDir" -DCMAKE_TOOLCHAIN_FILE="$vcpkgToolchain" -DCMAKE_BUILD_TYPE=$buildType

Write-Host "`n=== Building Project ==="
cmake --build . --config $buildType

Write-Host "`n=== Installing Executable ==="
cmake --install . --config $buildType

Write-Host "`n=== Copying Required DLLs to $targetDir ==="
$targetDir = "$installDir/bin"

if (-not $targetDir) {
    Write-Error "ERROR: Target directory was not initialized."
    exit 1
}

# ---- Copy Required DLLs ----
# List of DLLs that follow suffix rule
$dllsWithSuffix = @(
    "opencv_core4",
    "opencv_imgproc4",
    "opencv_highgui4",
    "opencv_imgcodecs4",
    "opencv_videoio4",
    "opencv_dnn4"
    
)

# List of DLLs that never change (no 'd' suffix even in Debug)
$dllsFixedName = @(
    "jpeg62",
    "zlibd1",
    "libwebp",
    "libwebpdecoder",
    "libwebpdemux",
    "libwebpmux",
    "libsharpyuv",
    "liblzma",
    "tiffd",
    "libpng16d"
)


foreach ($dll in $dllsWithSuffix) {
    $dllName = if ($buildType -eq "Debug") { "$dll" + "$suffix.dll" } else { "$dll.dll" }
    $src = Join-Path $vcpkgBinDir $dllName
    $dest = Join-Path $targetDir $dllName

    if (Test-Path $src) {
        Copy-Item $src -Destination $dest -Force
        Write-Host "Copied $dllName"
    } else {
        Write-Warning "DLL not found: $dllName"
    }
}

foreach ($dll in $dllsFixedName) {
    $dllName = "$dll.dll"
    $src = Join-Path $vcpkgBinDir $dllName
    $dest = Join-Path $targetDir $dllName

    if (Test-Path $src) {
        Copy-Item $src -Destination $dest -Force
        Write-Host "Copied $dllName"
    } else {
        Write-Warning "DLL not found: $dllName"
    }
}

Write-Host "`n=== All Done... Completed Build ==="

