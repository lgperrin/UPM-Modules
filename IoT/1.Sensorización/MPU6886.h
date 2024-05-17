#ifndef __MPU6886_H__
#define __MPU6886_H__

#include <Wire.h>

#define MPU6886_GYRO_OFFSET_X_H  0x13
#define MPU6886_GYRO_OFFSET_X_L  0x14
#define MPU6886_GYRO_OFFSET_Y_H  0x15
#define MPU6886_GYRO_OFFSET_Y_L  0x16
#define MPU6886_GYRO_OFFSET_Z_H  0x17
#define MPU6886_GYRO_OFFSET_Z_L  0x18

#define MPU6886_SMPLRT_DIV       0x19

#define MPU6886_ACCEL_X_THR      0x20
#define MPU6886_ACCEL_Y_THR      0x21
#define MPU6886_ACCEL_Z_THR      0x22

#define MPU6886_CONFIG           0x1A
#define MPU6886_GYRO_CONFIG      0x1B
#define MPU6886_ACCEL_CONFIG     0x1C
#define MPU6886_ACCEL_CONFIG2    0x1D
#define MPU6886_FIFO_EN          0x23

#define MPU6886_INT_PIN_CFG      0x37
#define MPU6886_INT_ENABLE       0x38

#define MPU6886_ACCEL_XOUT_H     0x3B
#define MPU6886_ACCEL_XOUT_L     0x3C
#define MPU6886_ACCEL_YOUT_H     0x3D
#define MPU6886_ACCEL_YOUT_L     0x3E
#define MPU6886_ACCEL_ZOUT_H     0x3F
#define MPU6886_ACCEL_ZOUT_L     0x40

#define MPU6886_TEMP_OUT_H       0x41
#define MPU6886_TEMP_OUT_L       0x42

#define MPU6886_GYRO_XOUT_H      0x43
#define MPU6886_GYRO_XOUT_L      0x44
#define MPU6886_GYRO_YOUT_H      0x45
#define MPU6886_GYRO_YOUT_L      0x46
#define MPU6886_GYRO_ZOUT_H      0x47
#define MPU6886_GYRO_ZOUT_L      0x48

#define MPU6886_ADDRESS          0x68
#define MPU6886_ACCEL_INTEL_CTRL 0x69
#define MPU6886_USER_CTRL        0x6A
#define MPU6886_PWR_MGMT_1       0x6B
#define MPU6886_PWR_MGMT_2       0x6C

#define MPU6886_FIFO_COUNT_H     0x72
#define MPU6886_FIFO_COUNT_L     0x73
#define MPU6886_FIFO_R_W         0x74

#define MPU6886_WHOAMI           0x75


typedef struct T_Datos_MPU6886
{
  uint8_t aceleracion_X_H;
  uint8_t aceleracion_X_L;
  uint8_t aceleracion_Y_H;
  uint8_t aceleracion_Y_L;
  uint8_t aceleracion_Z_H;
  uint8_t aceleracion_Z_L;
  uint8_t temperatura_H;
  uint8_t temperatura_L;
  uint8_t giroscopo_X_H;
  uint8_t giroscopo_X_L;
  uint8_t giroscopo_Y_H;
  uint8_t giroscopo_Y_L;
  uint8_t giroscopo_Z_H;
  uint8_t giroscopo_Z_L;
} Datos_MPU6886;


class MPU6886_i2c
{
  public:
    MPU6886_i2c(uint8_t id=MPU6886_ADDRESS, TwoWire &puerto=Wire1);

    bool Inicializar(void);
    
    float Aceleracion_X(void);
    float Aceleracion_Y(void);
    float Aceleracion_Z(void);
    float Giroscopo_X(void);
    float Giroscopo_Y(void);
    float Giroscopo_Z(void);
    float Temperatura(void);
    
    float LeerDesplazamientoGiroscopo_X(void);
    float LeerDesplazamientoGiroscopo_Y(void);
    float LeerDesplazamientoGiroscopo_Z(void);
    void EscribirDesplazamientoGiroscopo_X(float desplazamiento);
    void EscribirDesplazamientoGiroscopo_Y(float desplazamiento);
    void EscribirDesplazamientoGiroscopo_Z(float desplazamiento);

    float Aceleracion_X_Buffer(Datos_MPU6886 &datos);
    float Aceleracion_Y_Buffer(Datos_MPU6886 &datos);
    float Aceleracion_Z_Buffer(Datos_MPU6886 &datos);
    float Giroscopo_X_Buffer(Datos_MPU6886 &datos);
    float Giroscopo_Y_Buffer(Datos_MPU6886 &datos);
    float Giroscopo_Z_Buffer(Datos_MPU6886 &datos);
    float Temperatura_Buffer(Datos_MPU6886 &datos);

    uint16_t LeerNumeroElementosPilaFIFO(void);
    void Habilitar_FIFO(void);
    void Deshabilitar_FIFO(void);
    void Inicializar_FIFO(void);
    Datos_MPU6886 LeerPilaFIFO(void);

    void HabilitarInterrupcionAcelerometro(uint8_t umbral);
    void DeshabilitarInterrupcionAcelerometro(void);


  private:
    uint8_t LeerByte(uint8_t direccion);
    void EscribirByte(uint8_t direccion, uint8_t dato);
    int16_t LeerEnteroConSigno16Bits(uint8_t direccion);
    void EscribirEnteroConSigno16Bits(uint8_t direccion, int16_t dato);
    void LeerBufferBytes(uint8_t direccion, uint16_t longitudBuffer, uint8_t* buffer);

    TwoWire *puerto_i2c;
    int id_dispositivo;
    
    float resolucionAceleracion;
    float resolucionGiroscopo;
};

#endif
