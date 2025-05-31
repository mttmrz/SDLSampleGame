@echo off
:: Esegui questo script con doppio clic

echo -------------------------------------
echo Controllo e installazione dipendenze
echo -------------------------------------
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0setup_and_run.ps1"
pause
