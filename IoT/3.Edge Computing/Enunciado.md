# Práctica de _Edge Computing_

## Objetivos

Los objetivos que se persiguen son:

* Implementar un sistema que adquiera y procese muestras de voz, partiendo de la señal proporcionada por el micrófono SPM1423 y presente en el módulo M5 Stick C Plus.
* Desarrollar y transferir al procesador ESP32 una red neuronal convolucional que sirva para reconocer diferentes comandos de control hablados.

## Flujo de Trabajo

1. Seleccionar y grabar comandos de voz
2. Implementar CNN
3. Implementar sistema completo en M5 Stick C Plus


## Material necesario

- [x] Visual Studio
- [x] Arduino IDE
- [x] Dispositivo M5 Stick C Plus

## Comandos de voz elegidos

| Comando de Voz | Descripción                                |
|----------------|--------------------------------------------|
| Up       | Indica dirección "arriba"            |
| Down         | Indica dirección "abajo"            |
| Stop         | Detiene una acción o comando en curso  |


## Funciones implementadas

| Nombre del Método  | Descripción |
|--------------------|-------------|
| `calcular_espectrograma` | Calcula el espectrograma de una señal de audio utilizando una ventana de Hamming y la transformada de Fourier a corto plazo. |
| `cargar_y_procesar_audios` | Carga archivos de audio desde una carpeta y aplica la función calcular espectrograma a cada uno para obtener sus espectrogramas |
| `aplicar_kmeans` | Realiza la clasificación no supervisada de los espectrogramas utilizando el algoritmo KMeans. |
| `create_model` | Crea un modelo de red neuronal convolucional 1D para la clasificación de los datos de audio. |
| `model.fit` | Entrena el modelo de red neuronal con 75 épocas y un tamaño del batch de 32. |
| `representative_dataset_gen` | Genera un conjunto de datos representativo necesario para la conversión del modelo a Tensor-Flow Lite. |
| `convert_to_tflite` | Convierte el modelo entrenado a un formato TensorFlow Lite con optimizaciones y cuantificación. |

Finalmente, el modelo obtiene un accuracy del 93% aproximadamente en el conjunto de entrenamiento, lo que quiere decir que es mejorable ya que probablemente
exista sobreajuste (overfitting). Ello podría deberse a consideraciones de diseño o bien al número de muestras de voz con las que se trabaja para entrenar la red neuronal.
