<#
==== INSTRUCTIONS ====
Run Powershell CLI as Administrator
Run "Set-ExecutionPolicy Unrestricted"
Shift-Right click on this file, Copy Path
Paste into Powershell CLI and remove quotes to Run
#>

$TEMP_DIR = $env:TEMP
$GLEW_VERSION     = "2.0.0"
$FREEGLUT_VERSION = "";
$FLATCC_VERSION 	= "0.4.1"
$SOIL_VERSION 		= "2.0.5" # TODO
$VC_DIR = "$($Env:VS140COMNTOOLS)..\..\VC\"

Add-Type -AssemblyName System.IO.Compression.FileSystem
function Unzip
{
    param([string]$zipfile, [string]$outpath)

    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipfile, $outpath)
}

Write-Host "Downloading GLEW"
# Hardcoded Sourceforge Mirror, might break in the future
Invoke-WebRequest "https://pilotfiber.dl.sourceforge.net/project/glew/glew/$($GLEW_VERSION)/glew-$($GLEW_VERSION)-win32.zip" -OutFile "$($TEMP_DIR)\glew-$($GLEW_VERSION).zip"
Remove-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)" -Recurse -Force | Out-Null
Unzip "$($TEMP_DIR)\glew-$($GLEW_VERSION).zip" "$($TEMP_DIR)\"

Write-Host "Installing GLEW"
Copy-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)\lib\Release\Win32\*" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)\bin\Release\Win32\*" "$($VC_DIR)\bin\" -Recurse -Force

Write-Host "Downloading FreeGLUT"
Invoke-WebRequest "http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip" -OutFile "$($TEMP_DIR)\freeglut.zip"
Remove-Item "$($TEMP_DIR)\freeglut" -Recurse -Force | Out-Null
Unzip "$($TEMP_DIR)\freeglut.zip" "$($TEMP_DIR)\"

Write-Host "Installing FreeGLUT"
Copy-Item "$($TEMP_DIR)\freeglut\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\lib\*.lib" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\bin\*.dll" "$($VC_DIR)\bin\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\lib\x64\*.lib" "$($VC_DIR)\lib\amd64\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\bin\x64\*.dll" "$($VC_DIR)\bin\amd64\" -Recurse -Force

Write-Host "Cloning FlatCC"
Remove-Item "$($TEMP_DIR)\flatcc" -Recurse -Force | Out-Null
git clone --depth=1 --branch="v$($FLATCC_VERSION)" "https://github.com/dvidelabs/flatcc.git" "$($TEMP_DIR)\flatcc" | Out-Null
New-Item -Type Directory "$($TEMP_DIR)\flatcc\build\x86" | Out-Null
New-Item -Type Directory "$($TEMP_DIR)\flatcc\build\x64" | Out-Null

Write-Host "Building FlatCC x86"
cd "$($TEMP_DIR)\flatcc\build\x86"
cmake -G "Visual Studio 14 2015" ..\.. | Out-Null
cmake --build . --target --config Release | Out-Null
Copy-Item "$($TEMP_DIR)\flatcc\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatcc\lib\Release\*" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatcc\bin\Release\*" "$($VC_DIR)\bin\" -Recurse -Force

Write-Host "Building FlatCC x64"
cd "$($TEMP_DIR)\flatcc\build\x64"
cmake -G "Visual Studio 14 2015 Win64" ..\.. | Out-Null
cmake --build . --target --config Release | Out-Null
Copy-Item "$($TEMP_DIR)\flatcc\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatcc\lib\Release\*" "$($VC_DIR)\lib\amd64\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatcc\bin\Release\*" "$($VC_DIR)\bin\amd64\" -Recurse -Force
