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

# 2. Installa MinGW e make se non presenti
foreach ($pkg in @("mingw", "make")) {
    if (-not (Get-Command $pkg -ErrorAction SilentlyContinue)) {
        Write-Host "$pkg non trovato. Installo..."
        choco install $pkg --yes
    } else {
        Write-Host "$pkg già presente."
    }
}

# 3. Percorsi base
$baseDir = $PSScriptRoot
$includeSDL2     = Join-Path $baseDir "SDL2-2.30.2\x86_64-w64-mingw32\include"
$libSDL2         = Join-Path $baseDir "SDL2-2.30.2\x86_64-w64-mingw32\lib"
$includeSDL2Img  = Join-Path $baseDir "SDL2_image-2.8.2\x86_64-w64-mingw32\include"
$libSDL2Img      = Join-Path $baseDir "SDL2_image-2.8.2\x86_64-w64-mingw32\lib"
$dlls            = @("SDL2.dll", "SDL2_image.dll")
$outputExe       = Join-Path $baseDir "game.exe"
$sourceFiles     = @("main.cpp", "Entity.cpp", "game.cpp") | ForEach-Object { "`"$baseDir\$_`"" }

# 4. Compilazione
$compileCmd = "g++ $($sourceFiles -join ' ') -I`"$includeSDL2`" -I`"$includeSDL2Img`" -L`"$libSDL2`" -L`"$libSDL2Img`" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows -o `"$outputExe`""

Write-Host "Compilo con:"
Write-Host $compileCmd
Invoke-Expression $compileCmd

# 5. Copia DLL (se presenti)
foreach ($dll in $dlls) {
    $src = Join-Path $baseDir "SDL2-2.30.2\x86_64-w64-mingw32\bin\$dll"
    if (-not (Test-Path $src)) {
        $src = Join-Path $baseDir "SDL2_image-2.8.2\x86_64-w64-mingw32\bin\$dll"
    }
    if (Test-Path $src) {
        Copy-Item -Force $src -Destination $baseDir
    } else {
        Write-Warning "$dll non trovata."
    }
}

# 6. Esecuzione
if (Test-Path $outputExe) {
    Write-Host "Esecuzione di game.exe..."
    & "$outputExe"
} else {
    Write-Error "Compilazione fallita. Executable non trovato."
}
