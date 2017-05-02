<#
==== INSTRUCTIONS ====
Run Powershell CLI as Administrator
Run "Set-ExecutionPolicy Unrestricted"
Shift-Right click on this file, Copy Path
Paste into Powershell CLI and remove quotes to Run
#>

$TEMP_DIR 			     = $env:TEMP
$GLEW_VERSION     	 = "2.0.0"
$FREEGLUT_VERSION 	 = "";
$FLATBUFFERS_VERSION = "1.6.0"
$FLATCC_VERSION 	   = "0.4.1"
$ZLIB_VERSION		     = "1.2.11"
$DEVIL_VERSION 		   = "1.8.0"
$VC_DIR = "$($Env:VS140COMNTOOLS)..\..\VC\"

Add-Type -AssemblyName System.IO.Compression.FileSystem
function Unzip
{
    param([string]$zipfile, [string]$outpath)

    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipfile, $outpath)
}

Write-Host "Downloading GLEW v$($GLEW_VERSION)"
# Hardcoded Sourceforge Mirror, might break in the future
Invoke-WebRequest "https://pilotfiber.dl.sourceforge.net/project/glew/glew/$($GLEW_VERSION)/glew-$($GLEW_VERSION)-win32.zip" -OutFile "$($TEMP_DIR)\glew-$($GLEW_VERSION).zip"
If (Test-Path "$($TEMP_DIR)\glew-$($GLEW_VERSION)") { Remove-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)" -Recurse -Force | Out-Null }
Unzip "$($TEMP_DIR)\glew-$($GLEW_VERSION).zip" "$($TEMP_DIR)\"

Write-Host "Installing GLEW"
Copy-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)\lib\Release\Win32\*" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\glew-$($GLEW_VERSION)\bin\Release\Win32\*" "$($VC_DIR)\bin\" -Recurse -Force

Write-Host "Downloading FreeGLUT"
Invoke-WebRequest "http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip" -OutFile "$($TEMP_DIR)\freeglut.zip"
If (Test-Path "$($TEMP_DIR)\freeglut") { Remove-Item "$($TEMP_DIR)\freeglut" -Recurse -Force | Out-Null }
Unzip "$($TEMP_DIR)\freeglut.zip" "$($TEMP_DIR)\"

Write-Host "Installing FreeGLUT"
Copy-Item "$($TEMP_DIR)\freeglut\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\lib\*.lib" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\bin\*.dll" "$($VC_DIR)\bin\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\lib\x64\*.lib" "$($VC_DIR)\lib\amd64\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\freeglut\bin\x64\*.dll" "$($VC_DIR)\bin\amd64\" -Recurse -Force

Write-Host "Downloading Zlib v$($ZLIB_VERSION)"
$ZLIB_VERSION_SHORT = $ZLIB_VERSION -Replace "\.",""
Invoke-WebRequest "http://zlib.net/zlib${ZLIB_VERSION_SHORT}.zip" -OutFile "$($TEMP_DIR)\zlib.zip"
If (Test-Path "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)") { Remove-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)" -Recurse -Force | Out-Null }
Unzip "$($TEMP_DIR)\zlib.zip" "$($TEMP_DIR)\"
New-Item -Type Directory "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x86" | Out-Null
New-Item -Type Directory "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x64" | Out-Null

Write-Host "Building Zlib x86"
cd "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x86"
cmake -G "Visual Studio 14 2015" ..\.. | Out-Null
cmake --build . --target --config Release | Out-Null
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\zlib.h" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x86\Release\*.lib" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x86\Release\*.dll" "$($VC_DIR)\bin\" -Recurse -Force

Write-Host "Building Zlib x64"
cd "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x64"
cmake -G "Visual Studio 14 2015 Win64" ..\.. | Out-Null
cmake --build . --target --config Release | Out-Null
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\zlib.h" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x64\zconf.h" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x64\Release\*.lib" "$($VC_DIR)\lib\amd64\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\zlib-$($ZLIB_VERSION)\build\x64\Release\*.dll" "$($VC_DIR)\bin\amd64\" -Recurse -Force

Write-Host "Downloading DevIL v$($DEVIL_VERSION)"
# Hardcoded Sourceforge Mirror, might break in the future
Invoke-WebRequest "https://superb-sea2.dl.sourceforge.net/project/openil/DevIL%20Windows%20SDK/$($DEVIL_VERSION)/DevIL-Windows-SDK-$($DEVIL_VERSION).zip" -OutFile "$($TEMP_DIR)\devil.zip"
If (Test-Path "$($TEMP_DIR)\DevIL Windows SDK") { Remove-Item "$($TEMP_DIR)\DevIL Windows SDK" -Recurse -Force | Out-Null }
Unzip "$($TEMP_DIR)\devil.zip" "$($TEMP_DIR)\"

Write-Host "Installing DevIL"
Copy-Item "$($TEMP_DIR)\DevIL Windows SDK\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\DevIL Windows SDK\lib\x86\Release\*.lib" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\DevIL Windows SDK\lib\x86\Release\*.dll" "$($VC_DIR)\bin\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\DevIL Windows SDK\lib\x64\Release\*.lib" "$($VC_DIR)\lib\amd64\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\DevIL Windows SDK\lib\x64\Release\*.dll" "$($VC_DIR)\bin\amd64\" -Recurse -Force

Write-Host "Cloning FlatBuffers v$($FLATBUFFERS_VERSION)"
If (Test-Path "$($TEMP_DIR)\flatbuffers") { Remove-Item "$($TEMP_DIR)\flatbuffers" -Recurse -Force | Out-Null }
git clone --depth=1 --branch="v$($FLATBUFFERS_VERSION)" "https://github.com/google/flatbuffers.git" "$($TEMP_DIR)\flatbuffers" | Out-Null
New-Item -Type Directory "$($TEMP_DIR)\flatbuffers\build\x86" | Out-Null
New-Item -Type Directory "$($TEMP_DIR)\flatbuffers\build\x64" | Out-Null

Write-Host "Building FlatBuffers x86"
cd "$($TEMP_DIR)\flatbuffers\build\x86"
cmake -G "Visual Studio 14 2015" ..\.. | Out-Null
cmake --build . --target --config Release | Out-Null
Copy-Item "$($TEMP_DIR)\flatbuffers\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatbuffers\build\x86\Release\*.lib" "$($VC_DIR)\lib\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatbuffers\build\x86\Release\*.exe" "$($VC_DIR)\bin\" -Recurse -Force

Write-Host "Building FlatBuffers x64"
cd "$($TEMP_DIR)\flatbuffers\build\x64"
cmake -G "Visual Studio 14 2015 Win64" ..\.. | Out-Null
cmake --build . --target --config Release | Out-Null
Copy-Item "$($TEMP_DIR)\flatbuffers\include\*" "$($VC_DIR)\include\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatbuffers\build\x64\Release\*.lib" "$($VC_DIR)\lib\amd64\" -Recurse -Force
Copy-Item "$($TEMP_DIR)\flatbuffers\build\x64\Release\*.exe" "$($VC_DIR)\bin\amd64\" -Recurse -Force

Write-Host "Cloning FlatCC v$($FLATCC_VERSION)"
If (Test-Path "$($TEMP_DIR)\flatcc") { Remove-Item "$($TEMP_DIR)\flatcc" -Recurse -Force | Out-Null }
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
