import matplotlib.pyplot as plt
from funciones import (load_data, crear_segmentos, verificar_sensor, eliminar_componentes_continuas, eliminar_pendiente, 
    calcular_autocorrelacion, encontrar_maximo_autocorrelacion, calcular_frecuencia_cardiaca, 
    calcular_saturacion_sangre, verificar_spo2)


def main():
    # "./datosParte_A/PPG_data1.csv"
    # "./datosParte_A/PPG_data2.csv"

    # Preámbulo: Solicitar al usuario que ingrese la ruta del archivo
    file_path = input("Ingrese la ruta del archivo CSV: ")

    data = load_data(file_path)
    segmentos = crear_segmentos(data)

    frecuencias_cardiacas = []
    spo2_values = []
    segmento_sin_continua_list = []
    segmento_sin_pendiente_list = []

    for i in range(0,len(segmentos)):
        # Extraer el segmento
        segmento = segmentos[i]
        # Verificación del sensor
        sensor_activo = verificar_sensor(segmento)
        # Algoritmo
        if sensor_activo:
            # Eliminar la componente continua
            segmento_sin_continua = eliminar_componentes_continuas(segmento)
            segmento_sin_continua_list.append(segmento_sin_continua)

            # Eliminar la pendiente para ambos canales
            segmento_sin_pendiente = eliminar_pendiente(segmento_sin_continua)
            segmento_sin_pendiente_list.append(segmento_sin_pendiente)

            # Calcular vector de valores de autocorrelación para ir_channel
            ir_autocorr = calcular_autocorrelacion(segmento_sin_pendiente)

            # Valor máximo de correlación
            m_maximo = encontrar_maximo_autocorrelacion(ir_autocorr)

            # Calcular frecuencia cardíaca
            frecuencia_cardiaca = calcular_frecuencia_cardiaca(m_maximo, ir_autocorr)
            frecuencias_cardiacas.append(frecuencia_cardiaca)

            if frecuencia_cardiaca == 0:
                spo2 = 0
            else:
                # Calcular SPO2
                spo2 = calcular_saturacion_sangre(segmento, segmento_sin_pendiente)

            spo2 = verificar_spo2(spo2, segmento_sin_pendiente)
            spo2_values.append(spo2)

            print(f"Segmento: {i}, Frecuencia cardíaca: {frecuencia_cardiaca}, SPO2: {spo2}")

    # Plotting
    plt.figure(figsize=(14, 10))

    # Frecuencia cardíaca
    plt.subplot(2, 2, 1)
    plt.plot(frecuencias_cardiacas, linestyle='-', color="blue")
    plt.title(f'Frecuencia Cardiaca de la muestra {file_path}')
    plt.xlabel('Número de Muestra')
    plt.ylabel('Frecuencia Cardiaca')

    # SPO2
    plt.subplot(2, 2, 2)
    plt.plot(spo2_values, linestyle='-', color="red")
    plt.title(f'SPO2 de la muestra {file_path}')
    plt.xlabel('Número de Muestra')
    plt.ylabel('SPO2')

    # segmento_sin_continua
    plt.subplot(2, 2, 3)
    for segment in segmento_sin_continua_list:
        plt.plot(segment, marker='', linestyle='-', alpha=0.7, color="grey")
    plt.title('Segmentos sin Componente Continua')
    plt.xlabel('Número de Muestra')
    plt.ylabel('Valor')
    plt.ylim([-6000, 3000])

    # segmento_sin_pendiente
    plt.subplot(2, 2, 4)
    for segment in segmento_sin_pendiente_list:
        plt.plot(segment, marker='', linestyle='-', alpha=0.7, color="grey")
    plt.title('Segmentos sin Pendiente')
    plt.xlabel('Número de Muestra')
    plt.ylabel('Valor')
    plt.ylim([-6000, 3000])

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()