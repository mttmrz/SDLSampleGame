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
$mainDir = Join-Path $baseDir "main"  # Cartella dove sono i file sorgente
$dlls = @("SDL2.dll", "SDL2_image.dll", "SDL2_ttf.dll")
$outputExe = Join-Path $baseDir "game.exe"

# 4. File sorgente nella cartella main
$sourceFiles = @()
$sourceFiles += "`"$(Join-Path $mainDir "main.cpp")`""
$sourceFiles += "`"$(Join-Path $mainDir "Entity.cpp")`""
$sourceFiles += "`"$(Join-Path $mainDir "game.cpp")`""
$sourceFiles += "`"$(Join-Path $mainDir "texture_loader.cpp")`""

# 5. Compilazione con percorsi corretti
# -I"$mainDir" per gli header locali (Entity.hpp, game.hpp, etc.)
# -I"$mainDir\SDL2" per gli header SDL2 (che sono in main\SDL2\)
# -L"$mainDir" per le librerie SDL2 (che sono direttamente in main\)
$compileCmd = "g++ $($sourceFiles -join ' ') -I`"$mainDir`" -I`"$mainDir\SDL2`" -L`"$mainDir`" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows -o `"$outputExe`""

Write-Host "Compilo con:"
Write-Host $compileCmd
Invoke-Expression $compileCmd

# 6. Verifica presenza DLL nella cartella main (dove si trovano realmente)
foreach ($dll in $dlls) {
    $dllPath = Join-Path $mainDir $dll
    if (-not (Test-Path $dllPath)) {
        Write-Warning "$dll non trovata in $mainDir"
    }
}

# 7. Esecuzione
if (Test-Path $outputExe) {
    Write-Host "Esecuzione di game.exe..."
    
    # Copia l'eseguibile nella cartella main dove si trovano le risorse
    $gameExeInMain = Join-Path $mainDir "game.exe"
    Copy-Item $outputExe $gameExeInMain -Force
    Write-Host "Copiato game.exe nella cartella main"
    
    # Esegui dalla cartella main dove ci sono tutte le risorse
    Set-Location $mainDir
    Write-Host "Working directory: $(Get-Location)"
    & ".\game.exe"
} else {
    Write-Error "Compilazione fallita. Executable non trovato."
}

Write-Host "Premere un tasto per continuare..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")