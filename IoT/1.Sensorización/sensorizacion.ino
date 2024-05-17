#include "M5StickCPlus.h"
#include "MPU6886.h"

// Llamada a la clase de MPU6886 que guarda al invocarse
MPU6886_i2c imu(MPU6886_ADDRESS, Wire1);
volatile uint32_t contadorInterrupciones= 0;

#define BOTON_ENCENDIDO_PULSACION_CORTA 2

void ResetearDispositivo(void)
{
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextFont(0);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.drawString("Reseteando...", 20, M5.Lcd.height()/3, 2);

    M5.Beep.tone(220);
    delay(200);
    M5.Beep.tone(880);
    delay(200);
    M5.Beep.tone(440);
    delay(200);
    M5.Beep.mute();
    
    M5.Lcd.fillScreen(TFT_BLACK);
    Serial.end();
    ESP.restart();
}




#define NUMERO_VALORES_MEDIA_GIROSCOPO  1000  

void CalcularMediaGiroscopo(float media_giroscopo[3])
{  
  media_giroscopo[0]= (float)0.0;
  media_giroscopo[1]= (float)0.0;
  media_giroscopo[2]= (float)0.0;

  for(uint16_t i=0;i<NUMERO_VALORES_MEDIA_GIROSCOPO;i++)
  {
    media_giroscopo[0]+= imu.Giroscopo_X();
    media_giroscopo[1]+= imu.Giroscopo_Y();
    media_giroscopo[2]+= imu.Giroscopo_Z();
  }
  media_giroscopo[0]/= (float)NUMERO_VALORES_MEDIA_GIROSCOPO;
  media_giroscopo[1]/= (float)NUMERO_VALORES_MEDIA_GIROSCOPO;
  media_giroscopo[2]/= (float)NUMERO_VALORES_MEDIA_GIROSCOPO;
}


void IRAM_ATTR InterrupcionUmbralAceleracion(void)
{
    contadorInterrupciones++;
}

void setup()
{
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setSwapBytes(true);
  M5.Lcd.fillScreen(BLACK);
  M5.Axp.ScreenBreath(50);
  M5.Beep.mute();

  M5.Lcd.setTextFont(0);
  M5.Lcd.setTextDatum(TL_DATUM);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString("Inicio MPU6886...", 10, 10);

  if(imu.Inicializar())
  {
    float media_giroscopo[3];

      M5.Lcd.drawString(" -> Correcto.", 10, 30);

      CalcularMediaGiroscopo(media_giroscopo);

      imu.EscribirDesplazamientoGiroscopo_X(-2.0*media_giroscopo[0]);
      imu.EscribirDesplazamientoGiroscopo_Y(-2.0*media_giroscopo[1]);
      imu.EscribirDesplazamientoGiroscopo_Z(-2.0*media_giroscopo[2]);

      M5.Lcd.setCursor(10, 60);
      M5.Lcd.printf("Media Giroscopo:");
      M5.Lcd.setCursor(10, 80);
      M5.Lcd.printf("%.02f %.02f %.02f", media_giroscopo[0], media_giroscopo[1], media_giroscopo[2]);

      imu.Habilitar_FIFO();

      pinMode(GPIO_NUM_35, INPUT);
      attachInterrupt(GPIO_NUM_35, InterrupcionUmbralAceleracion, FALLING);
      imu.HabilitarInterrupcionAcelerometro(128); 
    }
    else
      M5.Lcd.drawString(" -> ¡Error!", 10, 30);
}


void loop()
{
  if(imu.LeerNumeroElementosPilaFIFO()>0)
  {
    Datos_MPU6886 datos_imu;
    float gx, gy, gz, ax, ay, az, t;

    datos_imu= imu.LeerPilaFIFO();

    gx= imu.Giroscopo_X_Buffer(datos_imu);
    gy= imu.Giroscopo_Y_Buffer(datos_imu);
    gz= imu.Giroscopo_Z_Buffer(datos_imu);
    ax= imu.Aceleracion_X_Buffer(datos_imu);
    ay= imu.Aceleracion_Y_Buffer(datos_imu);
    az= imu.Aceleracion_Z_Buffer(datos_imu);
    t= imu.Temperatura_Buffer(datos_imu);
        
    M5.Lcd.setCursor(10, 110);
    M5.Lcd.printf("Num. int: %d", contadorInterrupciones);

    Serial.printf("%.02f\t%.02f\t%.02f\t%.02f\t%.02f\t%.02f\t%.02f\n", ax, ay, az, gx, gy, gz, t);
  }
  else
    delay(1);

  M5.update();
  if(M5.Axp.GetBtnPress()==BOTON_ENCENDIDO_PULSACION_CORTA)
    ResetearDispositivo();
}
