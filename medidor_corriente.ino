#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const float voltaje = 9.0;//voltaje constante para el calculo de potencia, en este caso es 9v pero hay que cambiar el valor si se cambia de fuente
const float sensibilidad = 0.185; // Sensor 5A


float I = 0.0;
float ajuste = 0.01; // Para corregir desvíos en lectura
float potencia = voltaje * I;

void setup() {
  Serial.begin(9600);
  u8g2.begin();
}

void loop() {
  I = promedio_I(2000);
  I -= ajuste;

  Serial.print("Intensidad: ");
if (I >= 0.03) {
  
  float potencia = voltaje * I;
  Serial.println(I);
  oled(potencia);
  delay(100);
} else {
  Serial.println("0.00");
  I = 0.00;
  oled(0.0); 
  delay(100);
}



}

float promedio_I(int muestras) {
  float sensorA0, suma = 0.0;
  for (int i = 0; i < muestras; i++) {
    sensorA0 = analogRead(A0) * (5.0 / 1023.0);
    suma += (sensorA0 - 2.5) / sensibilidad;
  }
  return suma / muestras;
}

void oled(float P) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.setCursor(0, 30);
  u8g2.print("I: ");
  u8g2.print(I, 3); // 3 decimales
  u8g2.print(" [A]");

  u8g2.setCursor(0, 50);
  u8g2.print("P: ");
  u8g2.print(P, 3); // Potencia con 3 decimales
  u8g2.print(" [W]");

  u8g2.sendBuffer();
}