@echo off
setlocal EnableExtensions
cd /d "%~dp0"

echo ==============================================
echo      PREPARATION DU PACK WINDOWS QUIZ
echo ==============================================
echo.

if not exist "hamza_tahiri_et_marwan_ouali.exe" (
    echo ERREUR: hamza_tahiri_et_marwan_ouali.exe introuvable a la racine.
    echo Compile d'abord le projet avec build_and_run_hamza_marwan.bat
    pause
    exit /b 1
)

if not exist "release_quiz_app" mkdir "release_quiz_app"

echo [1/4] Copie des fichiers de release...
copy /Y "hamza_tahiri_et_marwan_ouali.exe" "release_quiz_app\hamza_tahiri_et_marwan_ouali.exe" >nul

if not exist "release_quiz_app\Lancer_Quiz_Windows.bat" (
    (
        echo @echo off
        echo cd /d %%~dp0
        echo start "" "hamza_tahiri_et_marwan_ouali.exe"
    ) > "release_quiz_app\Lancer_Quiz_Windows.bat"
)

if not exist "release_quiz_app\README_LANCEMENT.txt" (
    (
        echo QUIZ APP WINDOWS
        echo.
        echo 1^) Double-cliquer sur Lancer_Quiz_Windows.bat
        echo 2^) Jouer
        echo.
        echo Aucune installation requise.
    ) > "release_quiz_app\README_LANCEMENT.txt"
)

echo [2/4] Deblocage securite (si necessaire)...
powershell -NoProfile -ExecutionPolicy Bypass -Command "Get-ChildItem -Path '%CD%\release_quiz_app' -Recurse -File -Include *.exe,*.bat | Unblock-File" >nul 2>&1

echo [3/4] Creation du ZIP...
if exist "release_quiz_app_windows.zip" del /f /q "release_quiz_app_windows.zip"
powershell -NoProfile -ExecutionPolicy Bypass -Command "Compress-Archive -Path 'release_quiz_app\*' -DestinationPath 'release_quiz_app_windows.zip' -Force"
if errorlevel 1 (
    echo ERREUR: impossible de creer le ZIP.
    pause
    exit /b 1
)

echo [4/4] Termine.
echo Pack pret: release_quiz_app_windows.zip
echo Dossier pret: release_quiz_app
echo.
echo Tu peux transferer le ZIP vers n'importe quel PC Windows.
pause
exit /b 0
