@echo off
echo ---------------------------------------
echo [1/3] Creando carpeta bin...
if not exist bin mkdir bin

echo [2/3] Compilando Itxcho...
:: Fíjate que ahora el output (-o) es bin/itxcho.exe
g++ Src/main.cpp Src/core/Game.cpp Src/core/Level.cpp Src/core/ParticleSystem.cpp Src/Entities/Actor.cpp Src/Entities/Player.cpp -Iinclude -IC:\SFML-2.6.1\include -LC:\SFML-2.6.1\lib -DSFML_STATIC -o bin/itxcho.exe -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg

if %errorlevel% neq 0 (
    echo [ERROR] La compilacion fallo.
    pause
    exit /b
)

echo [3/3] Copiando Assets a bin...
:: Copiamos la carpeta Assets completa dentro de bin
if exist Assets xcopy /E /I /Y "Assets" "bin\Assets" >nul

:: Copia la DLL de audio necesaria
if exist "C:\SFML-2.6.1\bin\openal32.dll" copy /Y "C:\SFML-2.6.1\bin\openal32.dll" "bin\" >nul

echo Listo! Ejecutando Itxcho...
echo ---------------------------------------
cd bin
itxcho.exe
cd ..