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

# 3. Percorsi base (tutti in $PSScriptRoot, cioè cartella dello script)
$baseDir = $PSScriptRoot
$dlls = @("SDL2.dll", "SDL2_image.dll")
$outputExe = Join-Path $baseDir "game.exe"
$sourceFiles = @("main.cpp", "Entity.cpp", "game.cpp") | ForEach-Object { "`"$baseDir\$_`"" }

# 4. Compilazione: include e lib sono nella stessa cartella (.) dove sono gli header SDL2 (SDL2/SDL.h)
# Usa -I"." per indicare la cartella corrente (che è $baseDir)
$compileCmd = "g++ $($sourceFiles -join ' ') -I`"$baseDir`" -L`"$baseDir`" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows -o `"$outputExe`""

Write-Host "Compilo con:"
Write-Host $compileCmd
Invoke-Expression $compileCmd

# 5. Copia DLL (le dll sono già nella cartella base, quindi no copia necessaria)
foreach ($dll in $dlls) {
    $dllPath = Join-Path $baseDir $dll
    if (-not (Test-Path $dllPath)) {
        Write-Warning "$dll non trovata in $baseDir"
    }
}




# 6. Esecuzione

if (Test-Path $outputExe) {
    Write-Host "Esecuzione di game.exe..."
    # Assicurati che la working directory sia corretta
    Set-Location $baseDir
    Write-Host "Working directory: $(Get-Location)"
    & ".\game.exe"
} else {
    Write-Error "Compilazione fallita. Executable non trovato."
}
