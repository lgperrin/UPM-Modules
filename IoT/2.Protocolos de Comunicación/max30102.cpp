
#include "max30102.h"

MAX30102_i2c::MAX30102_i2c(uint8_t id, TwoWire &puerto)
{
  id_dispositivo= id;
  puerto_i2c= &puerto;
}


bool MAX30102_i2c::DispositivoConectado(void)
{
  uint8_t part_id;
  
  LeerRegistro_MAX30102(REG_PART_ID, &part_id);
  return(part_id==0x15);
}


void MAX30102_i2c::Reset(void)
{
  uint8_t registro;
  
  LeerRegistro_MAX30102(REG_MODE_CONFIG, &registro);
  EscribirRegistro_MAX30102(REG_MODE_CONFIG, (registro | 0x40));

  do
  {
    delay(1);
    LeerRegistro_MAX30102(REG_MODE_CONFIG, &registro);
  }
  while(registro & 0x40);
}


void MAX30102_i2c::Arranque(void)
{
  uint8_t registro;
  
  LeerRegistro_MAX30102(REG_MODE_CONFIG, &registro);
  EscribirRegistro_MAX30102(REG_MODE_CONFIG, (registro & 0x7f));
}


void MAX30102_i2c::Parada(void)
{
  uint8_t registro;
  
  LeerRegistro_MAX30102(REG_MODE_CONFIG, &registro);
  EscribirRegistro_MAX30102(REG_MODE_CONFIG, (registro | 0x80));
}


bool MAX30102_i2c::HayNuevosDatos(void)
{
  uint8_t registro;
  uint8_t write_p, read_p;
  
  LeerRegistro_MAX30102(REG_FIFO_WR_PTR, &registro);
  write_p= registro & 0x1f;
  LeerRegistro_MAX30102(REG_FIFO_RD_PTR, &registro);
  read_p= registro & 0x1f;

  return(write_p!=read_p);
}


void MAX30102_i2c::Inicializacion(void)
{
  EscribirRegistro_MAX30102(REG_INTR_ENABLE_1, 0x00); // No interrupts
  EscribirRegistro_MAX30102(REG_INTR_ENABLE_2, 0x00);
  
  EscribirRegistro_MAX30102(REG_FIFO_WR_PTR, 0x00);  // FIFO write pointer=0
  EscribirRegistro_MAX30102(REG_OVF_COUNTER, 0x00);  // FIFO overflow counter=0
  EscribirRegistro_MAX30102(REG_FIFO_RD_PTR, 0x00);  // FIFO read pointer= 0
  
  EscribirRegistro_MAX30102(REG_FIFO_CONFIG, 0x00);  // sample avg = 1, fifo rollover=false, fifo almost full= 32
  EscribirRegistro_MAX30102(REG_MODE_CONFIG, 0x03);  // 0x02 for Red only, 0x03 for SpO2
  EscribirRegistro_MAX30102(REG_SPO2_CONFIG, 0x27);  // SPO2_ADC range= 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (411uS)
  
  EscribirRegistro_MAX30102(REG_LED1_PA, 0x23);      // 7mA for LED1
  EscribirRegistro_MAX30102(REG_LED2_PA, 0x23);      // 7mA for LED2
}

void MAX30102_i2c::Leer_FIFO_MAX30102(int32_t *p_red_led, int32_t *p_ir_led)
{
  union value_32bits
  {
    uint8_t bits_8[4];
    uint32_t bits_32;
  } value;

  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(REG_FIFO_DATA);
  puerto_i2c->endTransmission();
  if(puerto_i2c->requestFrom(id_dispositivo, 6)==6)
  { 
    uint8_t byte_lectura;

    value.bits_8[3]= 0x0;
    
    byte_lectura= puerto_i2c->read();
    value.bits_8[2]= byte_lectura & 0x03;
  
    byte_lectura= puerto_i2c->read();
    value.bits_8[1]= byte_lectura;
    
    byte_lectura= puerto_i2c->read();
    value.bits_8[0]= byte_lectura;
    
    *p_red_led= value.bits_32;
    

    byte_lectura= puerto_i2c->read();
    value.bits_8[2]= byte_lectura & 0x03;
  
    byte_lectura= puerto_i2c->read();
    value.bits_8[1]= byte_lectura;
    
    byte_lectura= puerto_i2c->read();
    value.bits_8[0]= byte_lectura;
    
    *p_ir_led= value.bits_32;
  }
  else
  {
    *p_red_led= 0;
    *p_ir_led= 0;
  }
    
  puerto_i2c->endTransmission();
}


void MAX30102_i2c::EscribirRegistro_MAX30102(uint8_t direccion, uint8_t dato)
{
  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->write(dato);
  puerto_i2c->endTransmission();
}


void MAX30102_i2c::LeerRegistro_MAX30102(uint8_t direccion, uint8_t *p_dato)
{
  puerto_i2c->beginTransmission(id_dispositivo);
  puerto_i2c->write(direccion);
  puerto_i2c->endTransmission();
  if(puerto_i2c->requestFrom(id_dispositivo, 1)==1)
  {
    uint8_t byte_lectura;
    
    byte_lectura= puerto_i2c->read();
    *p_dato= byte_lectura;
  }
  else
    *p_dato= 0;

  puerto_i2c->endTransmission();
}
