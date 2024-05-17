/*
* Maxim MAX30102
*/

#ifndef MAX30102_H_
#define MAX30102_H_

#include <Wire.h>

#define MAX30102_ADDRESS    0x57

//Direcciones de registros
#define REG_INTR_STATUS_1   0x00
#define REG_INTR_STATUS_2   0x01
#define REG_INTR_ENABLE_1   0x02
#define REG_INTR_ENABLE_2   0x03
#define REG_FIFO_WR_PTR     0x04
#define REG_OVF_COUNTER     0x05
#define REG_FIFO_RD_PTR     0x06
#define REG_FIFO_DATA       0x07
#define REG_FIFO_CONFIG     0x08
#define REG_MODE_CONFIG     0x09
#define REG_SPO2_CONFIG     0x0A
#define REG_LED1_PA         0x0C
#define REG_LED2_PA         0x0D
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR       0x1F
#define REG_TEMP_FRAC       0x20
#define REG_TEMP_CONFIG     0x21
#define REG_REV_ID          0xFE
#define REG_PART_ID         0xFF

class MAX30102_i2c
{
  public:
    MAX30102_i2c(uint8_t id, TwoWire &puerto);

    bool DispositivoConectado(void);
    void Reset(void);
    void Inicializacion(void);
    bool HayNuevosDatos(void);
    void Leer_FIFO_MAX30102(int32_t *p_red_led, int32_t *p_ir_led);
    void Arranque(void);
    void Parada(void);

  private:
    void EscribirRegistro_MAX30102(uint8_t direccion, uint8_t dato);
    void LeerRegistro_MAX30102(uint8_t direccion, uint8_t *dato);


    int id_dispositivo;
    TwoWire *puerto_i2c;
};

#endif /*  MAX30102_H_ */
