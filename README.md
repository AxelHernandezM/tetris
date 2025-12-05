# celeste
juego de Celeste
mr walterwair
g++ Src/main.cpp Src/core/Game.cpp Src/Entities/Actor.cpp Src/Entities/Player.cpp -Iinclude -IC:\SFML-2.6.1\include -LC:\SFML-2.6.1\lib -o juego.exe -lsfml-graphics -lsfml-window -lsfml-system
g++ Src/main.cpp Src/core/Game.cpp Src/Entities/Actor.cpp Src/Entities/Player.cpp -Iinclude -IC:\SFML-2.6.1\include -LC:\SFML-2.6.1\lib -DSFML_STATIC -o juego.exe -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32