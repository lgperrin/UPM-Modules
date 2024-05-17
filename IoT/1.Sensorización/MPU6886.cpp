#include "MPU6886.h"

MPU6886_i2c::MPU6886_i2c(uint8_t id, TwoWire &puerto)
{
  id_dispositivo= id;
  puerto_i2c= &puerto;

  resolucionAceleracion= (float)8.0/(float)32768.0;
  resolucionGiroscopo= (float)2000.0/(float)32768.0;
}


uint8_t MPU6886_i2c::LeerByte(uint8_t direccion)
{
  uint8_t byte;

  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->endTransmission();
  if(puerto_i2c->requestFrom(id_dispositivo, 1)==1)
    byte= puerto_i2c->read();
  else
    byte= 0;

  return(byte);
}

int16_t MPU6886_i2c::LeerEnteroConSigno16Bits(uint8_t direccion)
{
  int16_t dato;

  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->endTransmission();
  if(puerto_i2c->requestFrom(id_dispositivo, 2)==2)
  {
    uint8_t byte_H, byte_L;
  
    byte_H= puerto_i2c->read();
    byte_L= puerto_i2c->read();
    dato= (int16_t)byte_H<<8 | byte_L;
  }
  else
    dato= 0;

  return(dato);
}  

void MPU6886_i2c::LeerBufferBytes(uint8_t direccion, uint16_t longitudBuffer, uint8_t *buffer)
{
  uint16_t i= 0;

  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->endTransmission();
  puerto_i2c->requestFrom(id_dispositivo, longitudBuffer);
  while(puerto_i2c->available())
    buffer[i++]= puerto_i2c->read();
}  


void MPU6886_i2c::EscribirByte(uint8_t direccion, uint8_t dato)
{
  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->write(dato);
  puerto_i2c->endTransmission();
}


void MPU6886_i2c::EscribirEnteroConSigno16Bits(uint8_t direccion, int16_t dato)
{
  uint8_t dato_H= (uint8_t)((uint16_t)dato>>8);
  uint8_t dato_L= (uint8_t)(dato & 0xff);

  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->write(dato_H);
  puerto_i2c->write(dato_L);
  puerto_i2c->endTransmission();
}

void MPU6886_i2c::Habilitar_FIFO(void)
{
    EscribirByte(MPU6886_USER_CTRL, 0x40);
}


void MPU6886_i2c::Deshabilitar_FIFO()
{
    EscribirByte(MPU6886_USER_CTRL, 0x00);
}


void MPU6886_i2c::Inicializar_FIFO(void)
{
    uint8_t valor;

    valor= LeerEnteroConSigno16Bits(MPU6886_USER_CTRL);
    valor|= 0x04;
    EscribirByte(MPU6886_USER_CTRL, valor);
}


uint16_t MPU6886_i2c::LeerNumeroElementosPilaFIFO(void)
{
    uint16_t n_elementos= (uint16_t)LeerEnteroConSigno16Bits(MPU6886_FIFO_COUNT_H)/sizeof(Datos_MPU6886);
    
    return(n_elementos);
}


Datos_MPU6886 MPU6886_i2c::LeerPilaFIFO(void)
{
  Datos_MPU6886 datos;

  LeerBufferBytes(MPU6886_FIFO_R_W, sizeof(datos), (uint8_t *)&datos);

  return(datos);
}


// Completar las funciones a partir de este punto.


bool MPU6886_i2c::Inicializar(void)
{
    // Comprobar el identificador de dispositivo (registro WHO_AM_I)
    uint8_t id = LeerByte(MPU6886_WHOAMI);
    if (id != 0x19)
    {
        return false;
        
    }

    // Activar el reset del dispositivo (registro POWER_MANAGEMENT_1)
    EscribirByte(MPU6886_PWR_MGMT_1, 0x41);
    while(LeerByte(MPU6886_PWR_MGMT_1) == 0x80)
    {
      delay(10);
    }

    // Desactivar el reset del dispositivo
    EscribirByte(MPU6886_PWR_MGMT_1, 0x00);

    // Activar la selección automática del reloj del dispositivo (registro POWER_MANAGEMENT_1)
    EscribirByte(MPU6886_PWR_MGMT_1, 0x01);

    // Seleccionar la escala de medida del acelerómetro a 8G (registro ACCEL_CONFIG)
    EscribirByte(MPU6886_ACCEL_CONFIG, 0x10);

    // Seleccionar la escala del giroscopio en 2000 DPS y FCHOICE_B a 00 (registro GYRO_CONFIG)
    EscribirByte(MPU6886_GYRO_CONFIG, 0x18);

    // Establecer modo completo para la pila FIFO, FSYNC no empleado y DLPF_CFG a 001 (registro CONFIG)
    EscribirByte(MPU6886_CONFIG, 0x01);

    // Establecer el divisor de frecuencia de muestreo a 10 para obtener una salida de 100Hz (registro SMPLRT_DIV)
    EscribirByte(MPU6886_SMPLRT_DIV, 10);

    // Desactivar todas las interrupciones (registro INT_ENABLE)
    EscribirByte(MPU6886_INT_ENABLE, 0x00);

    // Establecer media de 4 muestras, ACCEL_FCHOICE_B = 0 y A_DLPF_CFG = 00 en el registro ACCEL_CONFIG_2
    EscribirByte(MPU6886_ACCEL_CONFIG2, 0x00);

    // Inhabilitar la pila FIFO (registro USER_CTRL)
    EscribirByte(MPU6886_USER_CTRL, 0x00);

    // Asociar el acelerómetro y el giroscopio a la pila FIFO (registro FIFO_EN)
    EscribirByte(MPU6886_FIFO_EN, 0x78);

    // Habilita WoM con configuraciones específicas
    EscribirByte(MPU6886_ACCEL_INTEL_CTRL, 0xE0);  
    return true;
}



float MPU6886_i2c::Aceleracion_X(void)
{
    // Leer los valores de aceleración en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_ACCEL_XOUT_H);

    // Calcular la aceleración en unidades de G
    float escala = 8.0;  // Escala configurada en G
    float aceleracion = static_cast<float>(rawValue) / 32768.0; // 2^15 para 8G

    return aceleracion;
}

float MPU6886_i2c::Aceleracion_Y(void)
{
    // Leer los valores de aceleración en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_ACCEL_YOUT_H);

    // Calcular la aceleración en unidades de G
    float escala = 8.0;  // Escala configurada en G
    float aceleracion = static_cast<float>(rawValue) / 32768.0; // 2^15 para 8G

    return aceleracion;
}

float MPU6886_i2c::Aceleracion_Z(void)
{
    // Leer los valores de aceleración en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_ACCEL_ZOUT_H);

    // Calcular la aceleración en unidades de G
    float escala = 8.0;  // Escala configurada en G
    float aceleracion = static_cast<float>(rawValue) / 32768.0; // 2^15 para 8G

    return aceleracion;
}



float MPU6886_i2c::Giroscopo_X(void)
{
    // Leer los valores del giroscopio en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_GYRO_XOUT_H);

    // Calcular la velocidad angular en grados por segundo (DPS)
    float escala = 2000.0;  // Escala configurada en 2000 DPS
    float velocidadAngular = static_cast<float>(rawValue) / 32768.0 * escala; // 2^15 para 2000 DPS

    return velocidadAngular;
}

float MPU6886_i2c::Giroscopo_Y(void)
{
    // Leer los valores del giroscopio en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_GYRO_YOUT_H);

    // Calcular la velocidad angular en grados por segundo (DPS)
    float escala = 2000.0;  // Escala configurada en 2000 DPS
    float velocidadAngular = static_cast<float>(rawValue) / 32768.0 * escala; // 2^15 para 2000 DPS

    return velocidadAngular;
}

float MPU6886_i2c::Giroscopo_Z(void)
{
    // Leer los valores del giroscopio en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_GYRO_ZOUT_H);

    // Calcular la velocidad angular en grados por segundo (DPS)
    float escala = 2000.0;  // Escala configurada en 2000 DPS
    float velocidadAngular = static_cast<float>(rawValue) / 32768.0 * escala; // 2^15 para 2000 DPS

    return velocidadAngular;
}



float MPU6886_i2c::Temperatura(void)
{
    // Leer los valores de temperatura en los registros
    int16_t rawValue = LeerEnteroConSigno16Bits(MPU6886_TEMP_OUT_H);

    // Calcular la temperatura según la ecuación de conversión
    float temperatura = (2620.0 / pow(2, 15)) * pow(1.01, (rawValue - 1));

    return temperatura;
}



float MPU6886_i2c::LeerDesplazamientoGiroscopo_X(void)
{
    int16_t raw_offset = LeerEnteroConSigno16Bits(MPU6886_GYRO_OFFSET_X_H);
    float resolution = 2000.0 / (1 << 15);  // Resolución para 2000 DPS
    return static_cast<float>(raw_offset) * resolution;
}

float MPU6886_i2c::LeerDesplazamientoGiroscopo_Y(void)
{
    int16_t raw_offset = LeerEnteroConSigno16Bits(MPU6886_GYRO_OFFSET_Y_H);
    float resolution = 2000.0 / (1 << 15);  // Resolución para 2000 DPS
    return static_cast<float>(raw_offset) * resolution;
}

float MPU6886_i2c::LeerDesplazamientoGiroscopo_Z(void)
{
    int16_t raw_offset = LeerEnteroConSigno16Bits(MPU6886_GYRO_OFFSET_Z_H);
    float resolution = 2000.0 / (1 << 15);  // Resolución para 2000 DPS
    return static_cast<float>(raw_offset) * resolution;
}



void MPU6886_i2c::EscribirDesplazamientoGiroscopo_X(float desplazamiento)
{
    float resolution = 2000.0 / (1 << 15);  // Resolución para 2000 DPS
    int16_t raw_offset = static_cast<int16_t>(desplazamiento / resolution);
    EscribirEnteroConSigno16Bits(MPU6886_GYRO_OFFSET_X_H, raw_offset);
}

void MPU6886_i2c::EscribirDesplazamientoGiroscopo_Y(float desplazamiento)
{
    float resolution = 2000.0 / (1 << 15);  // Resolución para 2000 DPS
    int16_t raw_offset = static_cast<int16_t>(desplazamiento / resolution);
    EscribirEnteroConSigno16Bits(MPU6886_GYRO_OFFSET_Y_H, raw_offset);
}

void MPU6886_i2c::EscribirDesplazamientoGiroscopo_Z(float desplazamiento)
{
    float resolution = 2000.0 / (1 << 15);  // Resolución para 2000 DPS
    int16_t raw_offset = static_cast<int16_t>(desplazamiento / resolution);
    EscribirEnteroConSigno16Bits(MPU6886_GYRO_OFFSET_Z_H, raw_offset);
}



float MPU6886_i2c::Aceleracion_X_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.aceleracion_X_H << 8) | datos.aceleracion_X_L;
    return static_cast<float>(raw_value) / (1 << 15) * 8.0; // Resolución para ±8G
}

float MPU6886_i2c::Aceleracion_Y_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.aceleracion_Y_H << 8) | datos.aceleracion_Y_L;
    return static_cast<float>(raw_value) / (1 << 15) * 8.0; // Resolución para ±8G
}

float MPU6886_i2c::Aceleracion_Z_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.aceleracion_Z_H << 8) | datos.aceleracion_Z_L;
    return static_cast<float>(raw_value) / (1 << 15) * 8.0; // Resolución para ±8G
}



float MPU6886_i2c::Giroscopo_X_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.giroscopo_X_H << 8) | datos.giroscopo_X_L;
    return static_cast<float>(raw_value) / (1 << 15) * 2000.0; // Resolución para ±2000 DPS
}

float MPU6886_i2c::Giroscopo_Y_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.giroscopo_Y_H << 8) | datos.giroscopo_Y_L;
    return static_cast<float>(raw_value) / (1 << 15) * 2000.0; // Resolución para ±2000 DPS
}

float MPU6886_i2c::Giroscopo_Z_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.giroscopo_Z_H << 8) | datos.giroscopo_Z_L;
    return static_cast<float>(raw_value) / (1 << 15) * 2000.0; // Resolución para ±2000 DPS
}



float MPU6886_i2c::Temperatura_Buffer(Datos_MPU6886 &datos)
{
    int16_t raw_value = (datos.temperatura_H << 8) | datos.temperatura_L;
    float uncalibrated_temp = (static_cast<float>(raw_value) / (1 << 8)) + 25.0;

    // Aplicar la transformación según la ecuación proporcionada
    float temperature = (2620.0 / static_cast<float>(1 << 15)) * pow(1.01, uncalibrated_temp - 1);

    return temperature;
}



void MPU6886_i2c::HabilitarInterrupcionAcelerometro(uint8_t umbral)
{
    // Habilitar la interrupción para los 3 ejes
    EscribirByte(MPU6886_INT_ENABLE, 0x38);  // Habilitar interrupción para X, Y, Z

    // Aplicar el mismo valor de umbral a los registros ACCEL_WOM_X_THR y ACCEL_WOM_Y_THR
    EscribirByte(MPU6886_ACCEL_X_THR, umbral);
    EscribirByte(MPU6886_ACCEL_Y_THR, umbral);
}



void MPU6886_i2c::DeshabilitarInterrupcionAcelerometro(void)
{
    // Desactivar la interrupción para los 3 ejes
    EscribirByte(MPU6886_INT_ENABLE, 0x00);  // Deshabilitar todas las interrupciones
}
