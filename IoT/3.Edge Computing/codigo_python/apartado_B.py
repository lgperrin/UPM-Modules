
############################################################################
# Autores del Código: Laura García Perrín y Prisco García-Martín Consuegra #
########################     Asignatura: IoT         #######################
############################################################################

# Paso 0: Importar las librerías necesarias
import os
import tensorflow as tf
import librosa
import numpy as np
from scipy.signal.windows import hamming
from sklearn.model_selection import train_test_split
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

# Paso 1: Preprocesamiento - Calcular Espectrogramas
def calcular_espectrograma(audio, sr, n_fft=256, hop_length=128):
    ventana_hamming = hamming(n_fft, sym=False)
    espectrograma = librosa.stft(audio, n_fft=n_fft, hop_length=hop_length, window=ventana_hamming)
    espectrograma = np.abs(espectrograma)
    return espectrograma

def cargar_y_procesar_audios(ruta_carpeta):
    espectrogramas = []
    for archivo in os.listdir(ruta_carpeta):
        if archivo.endswith('.wav'):
            ruta_archivo = os.path.join(ruta_carpeta, archivo)
            audio, sr = librosa.load(ruta_archivo, sr=None)
            espectrograma = calcular_espectrograma(audio, sr)
            espectrogramas.append(espectrograma)
    return espectrogramas


# Paso 2: Clasificación no supervisada por KMeans
def aplicar_kmeans(espectrogramas, num_clusters=4):
    # Aplanar los espectrogramas
    datos_aplanados = np.array([np.mean(espectrograma, axis=1) for espectrograma in espectrogramas])
    scaler = StandardScaler()
    datos_escalados = scaler.fit_transform(datos_aplanados)
    kmeans = KMeans(n_clusters=num_clusters, random_state=42)
    etiquetas = kmeans.fit_predict(datos_escalados)
    return etiquetas

# Paso 3: Función para crear la red neuronal
def create_model(input_shape, num_classes):
    model = tf.keras.models.Sequential([
        tf.keras.layers.InputLayer(input_shape=input_shape),

        tf.keras.layers.Conv1D(filters=64, kernel_size=3, activation='relu'),
        tf.keras.layers.MaxPooling1D(pool_size=2),
        tf.keras.layers.BatchNormalization(),

        tf.keras.layers.Conv1D(filters=128, kernel_size=3, activation='relu'),
        tf.keras.layers.MaxPooling1D(pool_size=2),
        tf.keras.layers.BatchNormalization(),

        tf.keras.layers.GlobalAveragePooling1D(),

        tf.keras.layers.Dense(128, activation='relu'),
        tf.keras.layers.Dropout(0.5),

        tf.keras.layers.Dense(num_classes, activation='softmax')
    ])

    model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
    return model
    

# Paso 4: Función de conversión a TensorFlow Lite
    
# 4.a. Función generadora para el conjunto de datos representativo
def representative_dataset_gen():
    for value in X_train[:100]:  # Use a small portion of X_train as representative data
        # Ensure the data is in the correct shape and type
        yield [np.array(value, dtype=np.float32).reshape(1, -1, 1)]  # Reshape to [1, steps, input_dim]

# 4.b. Convert to TFLite
def convert_to_tflite(model, representative_data_gen):
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_data_gen
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.float32  # Input as 32-bit floats
    converter.inference_output_type = tf.float32  # Output as 32-bit floats
    tflite_model = converter.convert()
    with open('model.tflite', 'wb') as f:
        f.write(tflite_model)


# Cargar y procesar audios
ruta_carpeta_audios = "./apartado_A"
espectrogramas = cargar_y_procesar_audios(ruta_carpeta_audios)

# Aplicar KMeans para obtener etiquetas (si es necesario)
etiquetas = aplicar_kmeans(espectrogramas)

# Dividir en conjunto de entrenamiento y prueba
X_train, X_test, y_train, y_test = train_test_split(espectrogramas, etiquetas, test_size=0.2, random_state=42)

# Ajustar la forma de los datos para el modelo
X_train = np.array(X_train).reshape(len(X_train), -1, 1)  # Ajustar según la forma requerida
X_test = np.array(X_test).reshape(len(X_test), -1, 1)  # Ajustar según la forma requerida

# Crear el modelo
num_classes = len(np.unique(etiquetas))
model = create_model(X_train.shape[1:], num_classes)

# Entrenar el modelo
model.fit(X_train, y_train, validation_split=0.2, epochs=100, batch_size=32)
model.history

# Evaluate the model on the test data using `evaluate`
print("Evaluate on test data")
results = model.evaluate(X_test, y_test, batch_size=128)
print("test loss, test acc:", results)

# Guardar modelo
convert_to_tflite(model, representative_dataset_gen)

# Imprimir los resultados de la clasificación no supervisada
# for nombre, etiqueta in zip(nombres_archivos, etiquetas):
    # print(f"{nombre}: Cluster {etiqueta}")