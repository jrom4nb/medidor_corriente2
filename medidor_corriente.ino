#include <U8g2lib.h>

// Configuración de pantalla 
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// --- CONSTANTES---
const float VCC_REAL = 5.05; 
const float SENSIBILIDAD = 0.185; // 185mV/A para ACS712-5A
const float VOLTAJE_FUENTE = 9.0; // Fuente externa medida

float puntoCero = 0.0; // Variable para almacenar la calibración inicial
float corriente = 0.0;
float potencia = 0.0;

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  
  
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 20);
  u8g2.print("Calibrando...");
  u8g2.setCursor(0, 40);
  u8g2.print("Desconecte carga");
  u8g2.sendBuffer();
  
  long acumulador = 0;
  for(int i = 0; i < 100; i++) {
    acumulador += analogRead(A0);
    delay(10);
  }
  
  float promedioADC = acumulador / 100.0;
  puntoCero = promedioADC * (VCC_REAL / 1023.0);
  
  Serial.print("Calibración completada. Punto Cero (V): ");
  Serial.println(puntoCero, 4);
  delay(1000);
}

void loop() {
  corriente = leerCorriente(1000); 
  
  if (abs(corriente) < 0.03) {
    corriente = 0.0;
  }

  potencia = VOLTAJE_FUENTE * corriente;

  // Debug Serial
  Serial.print("I: ");
  Serial.print(corriente, 3);
  Serial.print(" A | P: ");
  Serial.println(potencia, 3);


  oled(corriente, potencia);
}

float leerCorriente(int muestras) {
  float voltajeSensor;
  float corrienteSuma = 0.0;
  long sumaADC = 0;
  
  for (int i = 0; i < muestras; i++) {
    sumaADC += analogRead(A0);
    delayMicroseconds(100); 
  }
  
  float promedioADC = sumaADC / (float)muestras;
  voltajeSensor = promedioADC * (VCC_REAL / 1023.0);
  return (voltajeSensor - puntoCero) / SENSIBILIDAD;
}

void oled(float I_val, float P_val) {
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_ncenB10_tr); 
  
  // Corrient
  u8g2.setCursor(0, 20);
  u8g2.print("I: ");
  u8g2.print(I_val, 3); 
  u8g2.print(" A");

  // Potencia
  u8g2.setCursor(0, 45);
  u8g2.print("P: ");
  u8g2.print(P_val, 3); 
  u8g2.print(" W");

  u8g2.sendBuffer();
}
