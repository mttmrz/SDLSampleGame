# PowerShell script: setup_and_run.ps1

Write-Host "-------------------------------------"
Write-Host "Controllo e installazione dipendenze"
Write-Host "-------------------------------------"

# 1. Installa Chocolatey se non presente
if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "Chocolatey non trovato. Installo..."
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
} else {
    Write-Host "Chocolatey già presente."
}

# 2. Installa MinGW se non presente
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "g++ non trovato. Installo MinGW..."
    choco install mingw --yes
} else {
    Write-Host "g++ già presente."
}

# 3. Installa make se non presente
if (-not (Get-Command make -ErrorAction SilentlyContinue)) {
    Write-Host "make non trovato. Installo make..."
    choco install make --yes
} else {
    Write-Host "make già presente."
}

# 4. Setup PATH MinGW (aggiunta temporanea se necessario)
$mingwBin64 = "C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin"
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    if (Test-Path "$mingwBin64\g++.exe") {
        Write-Host "Aggiungo $mingwBin64 al PATH temporaneamente..."
        $env:PATH += ";$mingwBin64"
    } else {
        Write-Error "g++ non trovato nemmeno in $mingwBin64. Interrompo."
        exit 1
    }
}

# 5. Funzione per scaricare ed estrarre SDL2 e SDL2_image (64bit)
function Install-SDL2 {
    param (
        [string]$name,
        [string]$url
    )
    $destDir = "C:\Libraries\$name"
    if (-not (Test-Path $destDir)) {
        Write-Host "Scarico e installo $name..."
        $zipPath = "$env:TEMP\$name.zip"
        Invoke-WebRequest -Uri $url -OutFile $zipPath
        Expand-Archive -Path $zipPath -DestinationPath $destDir
        Remove-Item $zipPath
    } else {
        Write-Host "$name già presente."
    }
}

Install-SDL2 -name "SDL2" -url "https://github.com/libsdl-org/SDL/releases/download/release-2.30.2/SDL2-devel-2.30.2-mingw-w64.zip"
Install-SDL2 -name "SDL2_image" -url "https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-devel-2.8.2-mingw-w64.zip"

# 6. Imposta i path corretti per 64bit MinGW-w64
$includeSDL2 = "C:\Libraries\SDL2\SDL2-2.30.2\x86_64-w64-mingw32\include"
$libSDL2 = "C:\Libraries\SDL2\SDL2-2.30.2\x86_64-w64-mingw32\lib"
$includeSDL2Img = "C:\Libraries\SDL2_image\SDL2_image-2.8.2\x86_64-w64-mingw32\include"
$libSDL2Img = "C:\Libraries\SDL2_image\SDL2_image-2.8.2\x86_64-w64-mingw32\lib"

# 7. Percorsi file sorgente e output
$cppFiles = @("main.cpp","Entity.cpp","game.cpp") | ForEach-Object { "`"$PSScriptRoot\$_`"" }
$outputPath = Join-Path $PSScriptRoot "game.exe"

# 8. Comando di compilazione
$cmd = "g++ $($cppFiles -join ' ') -I`"$includeSDL2`" -I`"$includeSDL2\SDL2`" -I`"$includeSDL2Img`" -L`"$libSDL2`" -L`"$libSDL2Img`" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows -o `"$outputPath`""

Set-Location $PSScriptRoot

Write-Host "Compilo con:"
Write-Host $cmd

Invoke-Expression $cmd

# 9. Copia DLL SDL2 e SDL2_image nella cartella dell'exe (per evitare errori "DLL non trovata")
$dllSourceSDL2 = "C:\Libraries\SDL2\SDL2-2.30.2\x86_64-w64-mingw32\bin\SDL2.dll"
$dllSourceIMG = "C:\Libraries\SDL2_image\SDL2_image-2.8.2\x86_64-w64-mingw32\bin\SDL2_image.dll"
$dllDestDir = $PSScriptRoot

if (Test-Path $dllSourceSDL2) {
    Copy-Item -Force $dllSourceSDL2 -Destination $dllDestDir
} else {
    Write-Warning "SDL2.dll non trovata in $dllSourceSDL2"
}
if (Test-Path $dllSourceIMG) {
    Copy-Item -Force $dllSourceIMG -Destination $dllDestDir
} else {
    Write-Warning "SDL2_image.dll non trovata in $dllSourceIMG"
}

# 10. Esecuzione
if (Test-Path "$outputPath") {
    Write-Host "Esecuzione di game.exe..."
    & "$outputPath"
} else {
    Write-Error "Compilazione fallita. Executable non trovato."
}
