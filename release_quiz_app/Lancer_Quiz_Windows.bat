@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if not exist "hamza_tahiri_et_marwan_ouali.exe" (
    echo ERREUR: hamza_tahiri_et_marwan_ouali.exe introuvable.
    pause
    exit /b 1
)

start "Le Meilleur Quiz" "hamza_tahiri_et_marwan_ouali.exe"
exit /b 0
