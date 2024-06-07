# 🔥 Modelo de Incendio Forestal

Este repositorio contiene un proyecto desarrollado en NetLogo que simula la propagación de un incendio forestal. El modelo permite observar la influencia del viento, la actuación de bomberos y helicópteros de rescate, así como la supervivencia de animales y la preservación de zonas del bosque.

## 👯 Participantes

- Xiya Sun (c20c030)
- Yimín Zhou (c20c024)
- Alba María López González (c20c037)
- Laura García Perrín (c20c015)

## ℹ️ Descripción del Proyecto

El modelo simula un incendio forestal con varios agentes y parámetros que afectan su comportamiento. Los principales componentes del modelo son:

- **Animales**: Representados por `turtles` que huyen del fuego o mueren si se encuentran en el mismo `patch` que el fuego.
- **Bomberos y Helicópteros**: Agentes que se mueven hacia el fuego para apagarlo, cada uno con diferentes capacidades de acción.
- **Viento**: Agente que altera la propagación del fuego en el bosque.

## 👾 Implementación

El proyecto se ha implementado en NetLogo y consta de las siguientes funciones principales:

- `setup`: Inicializa el modelo.
- `go`: Controla la ejecución del modelo en cada tick.
- `run-animals`: Mueve a los animales lejos del fuego.
- `kill-animals`: Determina si los animales mueren debido al fuego.
- `check-rescue-placement`: Posiciona bomberos o helicópteros en el bosque.
- `add-rescue`: Crea nuevos bomberos o helicópteros.
- `move-rescue`: Mueve a los bomberos y helicópteros hacia el fuego.
- `pour-water`: Apaga el fuego y actualiza los contadores de fuegos apagados y árboles salvados.
- `desaparecer`: Elimina bomberos o helicópteros cuando se quedan sin vida.
- `add-wind`: Cambia la velocidad y dirección del viento, afectando la propagación del fuego.

## 💻 Uso

Para ejecutar el modelo, sigue estos pasos:

1. Abre NetLogo.
2. Carga el archivo `fire-mod-all.nlogo`.
3. Haz clic en el botón `setup` para inicializar el modelo.
4. Haz clic en el botón `go` para empezar la simulación.

## 😉 Contribuciones

Las contribuciones al proyecto son bienvenidas. Por favor, sigue los siguientes pasos:

1. Haz un fork del repositorio.
2. Crea una nueva rama (`git checkout -b feature-nueva-funcionalidad`).
3. Realiza los cambios necesarios y haz commit (`git commit -am 'Añadir nueva funcionalidad'`).
4. Haz push a la rama (`git push origin feature-nueva-funcionalidad`).
5. Abre un Pull Request.

## 📜 Licencia

Este proyecto está bajo la licencia [MIT](LICENSE).


