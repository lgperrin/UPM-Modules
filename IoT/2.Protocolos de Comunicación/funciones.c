#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>

// Definir tipo de datos
struct Data {
    double r_channel;
    double ir_channel;
};

// Paso 0a: Cargar los datos
std::vector<Data> load_data(const std::string& file_path) {
    std::vector<Data> data;
    std::ifstream file(file_path);

    std::string line;
    while (std::getline(file, line)) {
        Data d;
        // Parse the line and fill the data
        data.push_back(d);
    }

    return data;
}

// Paso 0b: Segmentar los datos
std::vector<std::vector<Data>> crear_segmentos(const std::vector<Data>& data, int segment_start = 100, int segment_length = 400) {
    std::vector<std::vector<Data>> segments;
    for (size_t i = 0; i < data.size(); i += segment_start) {
        segments.push_back(std::vector<Data>(data.begin() + i, data.begin() + std::min(i + segment_length, data.size())));
    }
    return segments;
}

// Paso 1: Verificar si el sensor está activo
bool verificar_sensor(const std::vector<Data>& segmento) {
    for (const auto& d : segmento) {
        if (d.r_channel < 30000) return false;
    }
    return true;
}


// Paso 2: Eliminar componentes continuas
std::vector<Data> eliminar_componentes_continuas(const std::vector<Data>& segmento) {
    double sum_r = 0.0, sum_ir = 0.0;
    for (const auto& d : segmento) {
        sum_r += d.r_channel;
        sum_ir += d.ir_channel;
    }

    double mean_r = sum_r / segmento.size();
    double mean_ir = sum_ir / segmento.size();

    std::vector<Data> copy = segmento; // This creates a copy of the segmento
    for (auto& d : copy) {
        d.r_channel -= mean_r;
        d.ir_channel -= mean_ir;
    }

    return copy;
}

// Paso 3: Eliminar pendiente

double calcular_pendiente(const std::vector<Data>& segmento, bool is_r_channel) {
    int N = segmento.size();
    double m1 = 0.0, m2 = 0.0;

    for (int i = 0; i < N; ++i) {
        double value = is_r_channel ? segmento[i].r_channel : segmento[i].ir_channel;
        m1 += (2 * (i + 1) - (N + 1)) * value;
        m2 += (2 * (i + 1) - (N + 1)) * (2 * (i + 1) - (N + 1));
    }

    return m1 / m2;
}

std::vector<Data> eliminar_pendiente(const std::vector<Data>& segmento) {
    double m_r_channel = calcular_pendiente(segmento, true);
    double m_ir_channel = calcular_pendiente(segmento, false);

    std::vector<Data> adjusted_segmento = segmento;
    for (size_t i = 0; i < segmento.size(); ++i) {
        adjusted_segmento[i].r_channel -= m_r_channel * (i + 1);
        adjusted_segmento[i].ir_channel -= m_ir_channel * (i + 1);
    }

    return adjusted_segmento;
}

// Paso 4: Calcular autocorrelación
std::vector<double> calcular_autocorrelacion(const std::vector<Data>& segmento, bool is_r_channel) {
    int N = segmento.size();
    std::vector<double> valores(N);
    for (int i = 0; i < N; ++i) {
        valores[i] = is_r_channel ? segmento[i].r_channel : segmento[i].ir_channel;
    }

    std::vector<double> correlaciones(N, 0.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - i; ++j) {
            correlaciones[i] += valores[j] * valores[j + i];
        }
        correlaciones[i] /= (N - i);
    }

    return correlaciones;
}

// Paso 5: Calcular máximo de autocorrelación != m=0

int encontrar_maximo_autocorrelacion(const std::vector<double>& correlaciones) {
    if (correlaciones.empty()) {
        return -1; // Return -1 to indicate an error or empty input
    }

    int i = 1;
    double min = correlaciones[i];
    while (i < correlaciones.size() && correlaciones[i] <= min) {
        min = correlaciones[i];
        i += 5;
    }

    double max = -10000000;
    while (i < correlaciones.size() && correlaciones[i] >= max) {
        max = correlaciones[i];
        i += 5;
    }

    int m_max = -1; // Initialize to an invalid value
    for (int k = std::max(0, i - 10); k < i && k < correlaciones.size(); ++k) {
        if (correlaciones[k] >= 0) {
            if (correlaciones[k] >= max) {
                max = correlaciones[k];
                m_max = k;
            }
        } else {
            std::cout << "No todas las correlaciones son positivas" << std::endl;
            return 0; // Return 0 as per the original Python code
        }
    }

    return m_max;
}

// Paso 6: Calcular Frecuencia Cardíaca (HR)

double calcular_frecuencia_cardiaca(int m, const std::vector<double>& correlaciones) {
    if (m == 0) {
        return -1.0; // Return -1 to indicate an error or None
    }

    double r_mmax = correlaciones[m];
    double r_0 = correlaciones[0];
    double HR = 60.0 * (100.0 / m);

    // Check if HR is within the practical range
    if (30.0 <= HR && HR <= 225.0) {
        // Check if the estimation is valid based on autocorrelation
        if ((r_mmax / r_0) > 0.35) {
            return round(HR, 2);
        } else {
            // Autocorrelation is not strong enough
            return 0.0;
        }
    } else {
        // HR is not in the practical range
        return 0.0;
    }
}

// Paso 7: Calcular SPO2

double calcular_saturacion_sangre(const std::vector<Data>& segmento, const std::vector<Data>& segmento_sin_pendiente) {
    int N = segmento_sin_pendiente.size();

    // Calculate RMS and mean of the channels
    double rms_r = 0.0, rms_ir = 0.0, media_r = 0.0, media_ir = 0.0;
    for (int i = 0; i < N; ++i) {
        rms_r += std::pow(segmento_sin_pendiente[i].r_channel, 2);
        rms_ir += std::pow(segmento_sin_pendiente[i].ir_channel, 2);
        media_r += segmento[i].r_channel;
        media_ir += segmento[i].ir_channel;
    }

    rms_r = std::sqrt(rms_r / N);
    rms_ir = std::sqrt(rms_ir / N);
    media_r /= N;
    media_ir /= N;

    double z = (rms_r / media_r) / (rms_ir / media_ir);
    double spo2 = (-45.060 * z + 30.354) * z + 94.845;

    return spo2;
}


// Paso 8: Verificar SPO2
double verificar_spo2(double spo2, const std::vector<Data>& segmento_sin_pendiente) {
    int N = segmento_sin_pendiente.size();

    double numerador = 0.0, denominador1 = 0.0, denominador2 = 0.0;
    for (int i = 0; i < N; ++i) {
        numerador += segmento_sin_pendiente[i].r_channel * segmento_sin_pendiente[i].ir_channel;
        denominador1 += std::sqrt(segmento_sin_pendiente[i].r_channel * segmento_sin_pendiente[i].ir_channel);
        denominador2 += std::sqrt(segmento_sin_pendiente[i].ir_channel * segmento_sin_pendiente[i].r_channel);
    }

    double p = numerador / (denominador1 * denominador2);
    if (p > 0.8) {
        return spo2;
    } else {
        return 0.0;
    }
}

// ALGORITMO

int main() {
    // Preámbulo: Solicitar al usuario que ingrese la ruta del archivo
    std::string file_path;
    std::cout << "Ingrese la ruta del archivo CSV: ";
    std::cin >> file_path;

    std::vector<std::vector<double>> data = load_data(file_path);
    std::vector<std::vector<double>> segmentos = crear_segmentos(data);

    std::vector<double> frecuencias_cardiacas;
    std::vector<double> spo2_values;
    std::vector<std::vector<double>> segmento_sin_continua_list;
    std::vector<std::vector<double>> segmento_sin_pendiente_list;

    for (int i = 0; i < segmentos.size(); ++i) {
        // Extraer el segmento
        std::vector<double> segmento = segmentos[i];
        // Verificación del sensor
        bool sensor_activo = verificar_sensor(segmento);
        // Algoritmo
        if (sensor_activo) {
            // Eliminar la componente continua
            std::vector<double> segmento_sin_continua = eliminar_componentes_continuas(segmento);
            segmento_sin_continua_list.push_back(segmento_sin_continua);

            // Eliminar la pendiente para ambos canales
            std::vector<double> segmento_sin_pendiente = eliminar_pendiente(segmento_sin_continua);
            segmento_sin_pendiente_list.push_back(segmento_sin_pendiente);

            // Calcular vector de valores de autocorrelación para ir_channel
            std::vector<double> ir_autocorr = calcular_autocorrelacion(segmento_sin_pendiente);

            // Valor máximo de correlación
            double m_maximo = encontrar_maximo_autocorrelacion(ir_autocorr);

            // Calcular frecuencia cardíaca
            double frecuencia_cardiaca = calcular_frecuencia_cardiaca(m_maximo, ir_autocorr);
            frecuencias_cardiacas.push_back(frecuencia_cardiaca);

            if (frecuencia_cardiaca == 0) {
                spo2_values.push_back(0);
            } else {
                // Calcular SPO2
                double spo2 = calcular_saturacion_sangre(segmento, segmento_sin_pendiente);
                spo2 = verificar_spo2(spo2, segmento_sin_pendiente);
                spo2_values.push_back(spo2);
            }

            std::cout << "Segmento: " << i << ", Frecuencia cardíaca: " << frecuencia_cardiaca << ", SPO2: " << spo2_values.back() << std::endl;
        }
    }

    // Plotting (you'll need to implement this function)
    plot_data(frecuencias_cardiacas, spo2_values, segmento_sin_continua_list, segmento_sin_pendiente_list, file_path);

    return 0;
}