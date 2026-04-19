CUsershamzamini-projetbuild_and_run_hamza_marwan.bat@echo off
setlocal EnableExtensions
cd /d "%~dp0"

set "GCC=gcc"
if exist "C:\msys64\mingw64\bin\gcc.exe" set "GCC=C:\msys64\mingw64\bin\gcc.exe"
if exist "C:\msys64\ucrt64\bin\gcc.exe" set "GCC=C:\msys64\ucrt64\bin\gcc.exe"

echo Dossier courant: %CD%
echo Compilateur cible: %GCC%
echo.

if /I "%GCC%"=="gcc" (
    where gcc >nul 2>&1
    if errorlevel 1 (
        echo ERREUR: GCC introuvable dans le PATH.
        echo Installez MSYS2 MinGW64 puis relancez ce fichier.
        pause
        exit /b 1
    )
) else (
    if not exist "%GCC%" (
        echo ERREUR: GCC introuvable a ce chemin: %GCC%
        pause
        exit /b 1
    )
)

echo [1/3] Nettoyage...
if exist hamza_tahiri_et_marwan_ouali.exe del /f /q hamza_tahiri_et_marwan_ouali.exe

echo [2/3] Compilation du jeu...
"%GCC%" -std=c11 -Wall -Wextra -O2 src\main.c src\game.c src\input.c src\render.c src\persistence.c src\audio.c -o hamza_tahiri_et_marwan_ouali.exe > build.log 2>&1
if errorlevel 1 (
    echo.
    echo ERREUR: la compilation a echoue.
    echo Verifiez que GCC + Raylib sont installes (MSYS2/MinGW64).
    echo Affichage des erreurs:
    type build.log
    pause
    exit /b 1
)

if not exist hamza_tahiri_et_marwan_ouali.exe (
    echo.
    echo ERREUR: compilation terminee mais aucun .exe genere.
    echo Affichage du log:
    type build.log
    pause
    exit /b 1
)

echo [3/3] Lancement...
start "Hamza Tahiri et Marwan Ouali" hamza_tahiri_et_marwan_ouali.exe

echo Termine. Exe cree: hamza_tahiri_et_marwan_ouali.exe
pause
exit /b 0
