// BIBLIOTECAS Y CÓDIGOS FUENTE
#include "M5StickCPlus.h"
#include <Wire.h>
#include <WiFi.h>
#include "max30102.h"
#include "hr_spo2.h"
#include <PubSubClient.h>


// VARIABLES GLOBALES y CONSTANTES
#define BOTON_ENCENDIDO_PULSACION_CORTA 2

#define EXT_I2C_SDA   G32
#define EXT_I2C_SCL   G33
#define FREQ_HZ       400000


const char *ssid     = "Red_IoT";
const char *password = "12345678";


#define LONGITUD_BUFFER_DATOS_SENSOR       512
int32_t dato_r_sensor[LONGITUD_BUFFER_DATOS_SENSOR];
int32_t dato_ir_sensor[LONGITUD_BUFFER_DATOS_SENSOR];

#define DESPLAZAMIENTO_VENTANA_TRABAJO        (VENTANA_TRABAJO/4)
int32_t datoCanal_R[VENTANA_TRABAJO];
int32_t datoCanal_IR[VENTANA_TRABAJO];

volatile uint16_t n_escrituraSensor;
uint16_t n_lecturaSensor;   
uint16_t n_muestraCanales;

MAX30102_i2c sensor(MAX30102_ADDRESS, Wire);

#define UMBRAL_S   (int32_t)30000  // Umbral minima muestra canal R
#define UMBRAL_R   (float)0.4      // Umbral autocorelacion
#define UMBRAL_P   (float)0.8      // Umbral correlacion de Pearson


#define LONGITUD_BUFFER_MENSAJES_MQTT        128
float datos_hr[LONGITUD_BUFFER_MENSAJES_MQTT];
float datos_spo2[LONGITUD_BUFFER_MENSAJES_MQTT];

volatile uint16_t n_escritura_MQTT;
uint16_t n_lectura_hr_MQTT;   
uint16_t n_lectura_spo2_MQTT;   

const char* mqtt_server = "direccion_servidor_mqtt";
const int mqtt_port = 1883;


// FUNCIONES 
WiFiClient clienteWiFi; 
PubSubClient client(clienteWiFi);
client.setServer(mqtt_server, mqtt_port);


void Conexion_WiFi(void)
{
  uint16_t ciclosEsperaConexionWiFi= 100;

  Serial.print("Conectando WiFi...");

  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED && ciclosEsperaConexionWiFi)
  {
    if(WiFi.status()==WL_CONNECT_FAILED || WiFi.status()==WL_NO_SSID_AVAIL) 
      WiFi.begin(ssid, password);
    
    ciclosEsperaConexionWiFi--;
    Serial.print(".");
    delay(100);
  }
  
  if(WiFi.status()== WL_CONNECTED)
  {
    Serial.println();
    Serial.println("---------------");
    Serial.print("Conectado: ");
    Serial.println(WiFi.localIP());
    Serial.println("---------------");  
  }
}

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

void LeerDatos_Max3012(void *parametro)
{
  for( ; ; )
  { 
    if(sensor.HayNuevosDatos())
    {
      sensor.Leer_FIFO_MAX30102(&dato_r_sensor[n_escrituraSensor], &dato_ir_sensor[n_escrituraSensor]);
      if(n_escrituraSensor== (LONGITUD_BUFFER_DATOS_SENSOR-1))
        n_escrituraSensor= 0;
      else
        n_escrituraSensor++;
    }
    else
      delay(10);
  }
}


// Faltan las funciones auxiliares referidas al cliente MQTT

void conservarUltimasMuestras() {
    // Desplaza las últimas 300 muestras al inicio del array
    for (int i = 0; i < 300; i++) {
        datoCanal_R[i] = datoCanal_R[i + 100];
        datoCanal_IR[i] = datoCanal_IR[i + 100];
    }
    n_muestraCanales = 300;
}

//  función que maneje la conexión con el broker MQTT. Esta función intentará establecer una conexión y se reintentará si falla
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("arduinoClient")) {
      Serial.println("conectado");
      // Aquí puedes suscribirte a tópicos si es necesario
      // client.subscribe("tu/topico");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}



void setup()
{
  uint8_t ciclosEsperaConexionWiFi;
  
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
  M5.Lcd.drawString("MAX30102 HR & SpO2", 10, 10);

  Serial.println(""); 
  Serial.println("");
  Serial.println("Ejecutando el programa!");

  Conexion_WiFi();

  Wire.begin(EXT_I2C_SDA, EXT_I2C_SCL, FREQ_HZ);

  if(sensor.DispositivoConectado())
  {
    Serial.println("Encontrado el MAX30102.");

    sensor.Reset();
    Serial.println("Finalizada operacion de reset.");

    sensor.Inicializacion();
    Serial.println("Finalizada la inicializacion.");

    
    xTaskCreate(
      LeerDatos_Max3012,          
      "LeerDatos_Max3012",        
      4096,                      
      NULL,                       
      1,                         
      NULL                       
    );
 
    xTaskCreate(
      ProcesarDatos_Max3012,       
      "ProcesarDatos_Max3012",     
      4096,                        
      NULL,                       
      1,                          
      NULL                        
    );                     
    

  // Falta la inicialización del cliente MQTT, ...
  // activar la suscripción y ...
  // establecer la función de recepción de mensajes de entrada



  }
  else
  {
    M5.Lcd.drawString("Error deteccion MAX30102.", 10, 40);
    Serial.println("ERROR. No se detecta el MAX30102!");
    Serial.flush();
  }
 
  n_muestraCanales= 0;
  n_escrituraSensor= 0;
  n_lecturaSensor= 0;

  n_escritura_MQTT= 0;
  n_lectura_hr_MQTT= 0;
  n_lectura_spo2_MQTT= 0;
}





void ProcesarDatos_Max3012(void *parametro)
{
  for( ; ; )
  {
    if(n_muestraCanales<VENTANA_TRABAJO)
    {
      if(n_lecturaSensor!=n_escrituraSensor)
      {
        datoCanal_R[n_muestraCanales]= dato_r_sensor[n_lecturaSensor];
        datoCanal_IR[n_muestraCanales]= dato_ir_sensor[n_lecturaSensor];
        n_muestraCanales++;
      
        if(n_lecturaSensor== (LONGITUD_BUFFER_DATOS_SENSOR-1))
          n_lecturaSensor= 0;
        else
          n_lecturaSensor++;
      }
      else
        delay(10);
    }
    else
    {
      float hr, spo2, r, p;
      int32_t min_ch;
      int16_t i;

      Calculate_HR_SpO2(datoCanal_R, datoCanal_IR, &min_ch, &hr, &r, &spo2, &p);

      // Falta conservar las 3/4 partes últimas de los datos y actualizar el valor de n_muestraCanales

      if(min_ch>=UMBRAL_S && r>=UMBRAL_R && p>=UMBRAL_P)
      {
        // Falta guardar los datos de pulso y saturación en el buffer de mensajes...
        // ... para su posterior envío a través del cliente MQTT 



        M5.Lcd.drawString(String(hr), 50, 50);
        M5.Lcd.drawString(String(spo2), 50, 80);

        Serial.println();
        Serial.printf("HR: %.2f\tSpO2: %.2f\n", hr, spo2);
        Serial.printf("\t--> min_r: %d\t\tHR: %.2f\t[%.2f]\t\tSpO2: %.2f\t[%.2f]\n", min_ch, hr, r, spo2, p);
      }
      else
      {
        M5.Lcd.drawString(" 0.00", 50, 50);
        M5.Lcd.drawString(" 0.00", 50, 80);

        Serial.printf("\t--> min_r: %d\t\tHR: %.2f\t[%.2f]\t\tSpO2: %.2f\t[%.2f]\n", min_ch, hr, r, spo2, p);
      }
    }
  }
}  


void loop()
{
  // Gestionar las listas de valores de HR y SpO2 para enviar los que estén pendientes.
  // Enviar de forma efectiva los mensajes MQTT y verificar errores de envío.
  // Chequear las posibles pérdidas de conexión WiFi y al propio broker 
  if (!client.connected()) {
  reconnect();
  }
   
  M5.update();
  if(M5.Axp.GetBtnPress()==BOTON_ENCENDIDO_PULSACION_CORTA)
    ResetearDispositivo();


  delay(50);
}

client.loop();
