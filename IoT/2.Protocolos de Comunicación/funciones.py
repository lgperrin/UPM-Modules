import numpy as np
import pandas as pd

# Paso 0: Cargar los datos
def load_data(file_path):
    data = pd.read_csv(file_path, sep=";")
    return data

# Paso 0b: Segmentar los datos
def crear_segmentos(data, segment_start=100, segment_length=400):
    return [data[i:i + segment_length] for i in range(0, len(data), segment_start)]

# Paso 1: Verificar si el sensor está activo o no 
def verificar_sensor(segmento):
    return segmento['r_channel'].min() >= 30000

# Paso 2: Eliminar la componente continua de las muestras de la ventana de trabajo
def eliminar_componentes_continuas(segmento):
    copy = segmento.copy()
    copy['r_channel'] -= segmento['r_channel'].mean()
    copy['ir_channel'] -= segmento['ir_channel'].mean()
    return copy

# Paso 3: Eliminar la pendiente de las señales del punto anterior 
def calcular_pendiente(segmento, canal):
    N = len(segmento)
    t = np.arange(1, N + 1)
    m1 = np.sum((2 * t - (N + 1)) * segmento[canal])
    m2 = np.sum((2 * t - (N + 1))**2)
    return m1 / m2

def eliminar_pendiente(segmento):
    m_r_channel = calcular_pendiente(segmento, "r_channel")
    m_ir_channel = calcular_pendiente(segmento, "ir_channel")
    t = np.arange(1, len(segmento) + 1)
    
    segmento['r_channel'] -= m_r_channel * t
    segmento['ir_channel'] -= m_ir_channel * t

    return segmento

# Paso 4: Calcular la autocorrelación del segmento
def calcular_autocorrelacion(segmento, channel="ir_channel"):
    valores = segmento[channel].values
    N = len(valores)
    correlaciones = np.correlate(valores, valores, mode='full')[N-1:] / np.arange(N, 0, -1)
    return correlaciones.tolist()

# Paso 5: Encontrar primer máximo de autocorrelación distinto de m=0
def encontrar_maximo_autocorrelacion(correlaciones):
    i = 1
    min = correlaciones[i]
    while correlaciones[i] <= min:
        min = correlaciones[i]
        i += 5
    
    max = -10000000
    while correlaciones[i] >= max:
        max = correlaciones[i]
        i += 5
    
    for k in range(i-10, i):
        if correlaciones[k] >= 0:
            if correlaciones[k] >= max:
                max = correlaciones[k]
                m_max = k
                k += 1
        else:
            print("No todas las correlaciones son positivas")
            return 0 # debe ser >= 0

    return m_max

# Paso 6: Estimar del valor del pulso cardiaco medido en pulsaciones por minuto
def calcular_frecuencia_cardiaca(m, correlaciones):
    if m == 0:
        return None
    r_mmax = correlaciones[m]
    r_0 = correlaciones[0]
    HR = 60 * (100 / m)
    # Verificar si HR está dentro del rango práctico
    if 30 <= HR <= 225:
        # Verificar si la estimación es válida según la autocorrelación
        if (r_mmax / r_0) > 0.35:
            return round(HR, 2)
        else:
            # La autocorrelación no es lo suficientemente fuerte
            return 0  
    else:
        # HR no está en el rango práctico
        return 0  
    
# Paso 7: Estimar % de saturación en sangre
def calcular_saturacion_sangre(segmento, segmento_sin_pendiente):
    N = len(segmento_sin_pendiente)
    # Calcular RMS y media de los canales
    rms_r = np.sqrt(sum(segmento_sin_pendiente['r_channel']**2)/N)
    rms_ir = np.sqrt(sum(segmento_sin_pendiente['ir_channel']**2)/N)
    media_r = segmento['r_channel'].sum()/N
    media_ir = segmento['ir_channel'].sum()/N

    z = (rms_r/media_r)/(rms_ir/media_ir)
    spo2 = (-45.060*z + 30.354)*z + 94.845 
    return spo2

# Paso 8: Verificar que el resultado del paso anterior 
def verificar_spo2(spo2, segmento_sin_pendiente):
    numerador = sum(segmento_sin_pendiente["r_channel"]*segmento_sin_pendiente["ir_channel"])
    denominador1 = np.sqrt(sum(segmento_sin_pendiente["r_channel"]*segmento_sin_pendiente["ir_channel"]))
    denominador2 = np.sqrt(sum(segmento_sin_pendiente["ir_channel"]*segmento_sin_pendiente["r_channel"]))

    p = numerador/(denominador1*denominador2)
    if p > 0.8:
        return spo2
    else:
        return 0 
