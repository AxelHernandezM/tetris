Objetivo del Juego
El objetivo principal es guiar al personaje a través de una serie de niveles de plataformas desafiantes. El jugador debe esquivar obstáculos mortales (como pinchos) y utilizar habilidades de movimiento avanzadas para alcanzar al alebrije al final de cada mapa. Opcionalmente, el jugador puede recolectar Limones dispersos por el nivel para aumentar su puntuación.

🎮 Controles
Modo Juego
Flechas (Izquierda/Derecha): Mover al personaje.

Z: Saltar (incluye salto de pared).

X: Realizar Dash (impulso aéreo).

Modo Editor de Niveles
E: Activar/Desactivar el Editor en tiempo real.

Click Izquierdo: Colocar bloque.

Click Derecho: Borrar bloque.

Tecla 1: Seleccionar pincel de Pasto (#).

Tecla 2: Seleccionar pincel de Pincho (^).

Tecla 3: Seleccionar pincel de Fresa (@).

Tecla 4: Seleccionar pincel de Ladrillo (B).

⚙️ Mecánicas
Físicas Pixel Perfect: Sistema de colisiones preciso AABB (Axis-Aligned Bounding Box).

Movimiento Avanzado: Implementación de mecánicas complejas como Wall Jump (salto de pared), Wall Slide (deslizarse por la pared) y Dash en 8 direcciones.

Game Feel: Incluye "Coyote Time" (tiempo de gracia para saltar al salir de una plataforma) y "Jump Buffer" (recordar el salto si se presiona antes de tocar el suelo).

Sistema de Partículas: Generación de polvo visual al aterrizar o saltar para mayor impacto visual.

Transición de Niveles: Sistema automático que carga el siguiente mapa al tocar la bandera y reposiciona al jugador en el punto de spawn ('P').

🏆 Características
Autotiling Básico: El juego detecta automáticamente si un bloque debe tener textura de "Pasto" (superficie) o "Tierra" (subsuelo) dependiendo de sus vecinos.

Editor en Tiempo Real: Permite modificar el nivel mientras se juega para pruebas rápidas y diseño.

Sistema de Audio: Soporte para música de fondo en bucle (.ogg) y efectos de sonido (.wav) para acciones como saltar, dash y recolectar.

Menú Principal: Pantalla de título con efectos visuales y gestión de estados del juego (Menú vs. Jugando).

Animaciones: Sprites animados para correr, inactividad (idle) y saltar.

👥 Equipo
Líder: Axel Alejandro Hernandez Manzano 

Integrante 2: FERNANDO GERMAN ESPINOZA PLASCENCIA 

🛠️ Tecnologías
Lenguaje: C++ (Standard 17 o superior recomendado).

Librería Gráfica: SFML 2.6.1 (Simple and Fast Multimedia Library).

Módulos SFML: Graphics, Window, System, Audio.

Recursos: Carga de fuentes .ttf, imágenes .png y audio.

📜 Créditos
Assets de terceros utilizados: Pixelvay (https://pixabay.com)

Referencias o inspiraciones: Inspirado en las mecánicas de Celeste.