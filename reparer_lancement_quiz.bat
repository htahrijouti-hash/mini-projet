@echo off
setlocal EnableExtensions
cd /d "%~dp0"

echo ==============================================
echo   REPARATION + LANCEMENT LE MEILLEUR QUIZ
echo ==============================================
echo.

set "TARGET=Le_Meilleur_Quiz_de_HM.exe"
if exist "%TARGET%" goto :found

if exist "Le_Meilleur_Quiz_App\Le_Meilleur_Quiz_de_HM.exe" (
    set "TARGET=Le_Meilleur_Quiz_App\Le_Meilleur_Quiz_de_HM.exe"
    goto :found
)

if exist "Le_Meilleur_Quiz_Share_Windows\Le_Meilleur_Quiz_de_HM.exe" (
    set "TARGET=Le_Meilleur_Quiz_Share_Windows\Le_Meilleur_Quiz_de_HM.exe"
    goto :found
)

if exist "Le_Meilleur_Quiz_PC_TL\PC_Windows\Le_Meilleur_Quiz_de_HM.exe" (
    set "TARGET=Le_Meilleur_Quiz_PC_TL\PC_Windows\Le_Meilleur_Quiz_de_HM.exe"
    goto :found
)

echo ERREUR: aucun executable du quiz trouve.
echo Verifie que le dossier du quiz est complet.
pause
exit /b 1

:found
echo Executable detecte: %TARGET%
echo.

echo [1/3] Deblocage de securite Windows (si necessaire)...
powershell -NoProfile -ExecutionPolicy Bypass -Command "Get-ChildItem -Path '%CD%' -Recurse -File -Include *.exe,*.bat | Unblock-File" >nul 2>&1

echo [2/3] Verification presence executable...
if not exist "%TARGET%" (
    echo ERREUR: executable introuvable apres verification.
    pause
    exit /b 1
)

echo [3/3] Lancement du quiz...
start "Le Meilleur Quiz" "%TARGET%"

echo.
echo Si Windows affiche un avertissement SmartScreen:
echo - Clique sur "Informations complementaires"
echo - Puis "Executer quand meme"
echo.
echo Si un antivirus bloque le fichier, ajoute ce dossier en exclusion.
pause
exit /b 0
