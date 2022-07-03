
//Definición de funciones y librerías
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Credenciales WiFi
 char auth[] = "-";
 char ssid[] = "-"; // 
 char pass[] = "-"; 

    //Declaración de variables y funciones
      #define SENSOR D2
      long Millis = 0;
      long ant_Millis = 0;
      int intervalo = 1000;
      boolean ledState = LOW;
      float FactorDeFlujo = 7.5;
      volatile byte Contador;
      byte pulse1Sec = 0;
      float Flujo;
      unsigned int ml_fluidos;
      unsigned long total_mL;
      
      //Función reloj
  void IRAM_ATTR Reloj()
  {
  Contador++;
   }

void setup()
{
 //Inicializa las variables del programa
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
         pinMode(SENSOR, INPUT_PULLUP);
         Contador = 0;
         Flujo = 0.0;
         ml_fluidos = 0;
         total_mL = 0;
         ant_Millis = 0;
         attachInterrupt(digitalPinToInterrupt(SENSOR), Reloj, FALLING);
}

void loop()
{
         Millis = millis();
         if (Millis - ant_Millis > intervalo) { 
         pulse1Sec = Contador;
         Contador = 0;
         Flujo = ((1000.0 / (millis() - ant_Millis)) * pulse1Sec) / FactorDeFlujo;
         ant_Millis = millis();
         ml_fluidos = (Flujo / 60) * 1000;
         total_mL += ml_fluidos;

 //Despliega los valores calculados a los servidores de Blynk
         Serial.print("Flow Rate: ");
         Serial.print(int(Flujo)); 
         Serial.print("L/min");
         Serial.print("\t"); 
         Serial.print("Output Liquid Quantity: ");
         Serial.print(total_mL);
         Serial.print("mL / ");
         Serial.print(total_mL / 1000);
         Serial.println("L");
         
//Escribe en los pines virtuales de Blynk para su interpretación en gráficos
 Blynk.virtualWrite(V2, Flujo);
 Blynk.virtualWrite(V3, total_mL);
 Blynk.virtualWrite(V4, Millis*1000);
 Blynk.run();
}
}
 
 