#include <LiquidCrystal.h>
int LedG = 21;
int LedY = 12;
int LedR = 2;
int pot = 36;
int LedU = 22;
int vent = 5;
int rs = 19, en = 23, d4 = 18, d5 = 17, d6 = 16, d7 = 15;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char auth[] = "qzN7eHKDAayY4bYkd9sHV_sjS8P-ul-j";
char ssid[] = "SSD_C3150";
char pass[] = "ssdsolo1238";

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

float sensorValue;
float Temp_min = 10.00;
float Temp_max = 50.00;

void dataSensor()
{
  float Humi = dht.readHumidity();
  float Temp = dht.readTemperature();

  if (isnan(Humi)

    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V7, sensorValue);
  sensorValue = float (map(analogRead(pot), 0, 4095, Temp_min, Temp_max));
  lcd.setCursor(8, 0);
  lcd.print(sensorValue);
  delay(50);

  if ((sensorValue >= Temp - 1) && (sensorValue <= Temp + 1))
  {
    digitalWrite(LedY, LOW);
    digitalWrite(LedG, HIGH);
    digitalWrite(vent, LOW);
    digitalWrite(LedR, LOW);
    digitalWrite(LedU, HIGH);
    Blynk.notify("tempratuur goed!");
    Blynk.virtualWrite(V0, Temp);
    Blynk.virtualWrite(V1, Humi);
    Blynk.virtualWrite(V2, Temp);
    Blynk.virtualWrite(V3, Humi);
    WidgetLED LedG (V6);
    LedG.on();
    WidgetLED LedR (V4);
    LedR.off();
    WidgetLED LedY (V5);
    LedY.off();
    WidgetLED LedU (V9);
    LedU.on();
    return;
  }
  else
  {
    if (Temp <= sensorValue)
    {
      digitalWrite(LedY, LOW);
      digitalWrite(LedG, LOW);
      digitalWrite(vent, LOW);
      digitalWrite(LedR, HIGH);
      digitalWrite(LedU, LOW);
      Blynk.notify("Verwaming Aan!");
      Blynk.virtualWrite(V0, Temp);
      Blynk.virtualWrite(V1, Humi);
      Blynk.virtualWrite(V2, Temp);
      Blynk.virtualWrite(V3, Humi);
      WidgetLED LedG (V6);
      LedG.off();
      WidgetLED LedR (V4);
      LedR.on();
      WidgetLED LedY (V5);
      LedY.off();
      WidgetLED LedU (V9);
      LedU.off();
      return;
    }
    else
    {
      digitalWrite(LedY, HIGH);
      digitalWrite(LedG, LOW);
      digitalWrite(vent, HIGH);
      digitalWrite(LedR, LOW);
      digitalWrite(LedU, LOW);
      Blynk.notify("Airco Aan!");
      Blynk.virtualWrite(V0, Temp);
      Blynk.virtualWrite(V1, Humi);
      Blynk.virtualWrite(V2, Temp);
      Blynk.virtualWrite(V3, Humi);
      WidgetLED LedG (V6);
      LedG.off();
      WidgetLED LedR (V4);
      LedR.off();
      WidgetLED LedY (V5);
      LedY.on();
      WidgetLED LedU (V9);
      LedU.off();
      return;
    }
  }
}
void setup()
{
  pinMode(pot, INPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedY, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(vent, OUTPUT);

  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 0, 110), 8080);

  dht.begin();
  lcd.begin(16, 2);

  timer.setInterval(1000L, dataSensor);
}

void loop()
{
  Blynk.run();
  timer.run();

  lcd.setCursor(0, 0);
  lcd.print("W Temp");
  lcd.setCursor(0, 1);
  lcd.print("M Temp");
  lcd.setCursor(14, 0);
  lcd.print((char)223);
  lcd.print("C");

  float Temp = dht.readTemperature();
  lcd.setCursor(8, 1);
  lcd.print(Temp);
  lcd.setCursor(14, 1);
  lcd.print((char)223);
  lcd.print("C");

}
