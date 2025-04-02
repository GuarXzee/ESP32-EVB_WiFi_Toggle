#include "Board_Pinout.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Wire.h"
#include "NS2009.h"

#define RELAY1 32
#define RELAY2 33

#define BOXSIZE 40

NS2009 TS(false, true);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
uint8_t tp[5];

void resetLeftButoon()
{
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.fillRoundRect(20, 200, 100, 100, 8, ILI9341_RED);
  tft.drawRoundRect(20, 200, 100, 100, 8, ILI9341_WHITE);
  tft.setCursor(45, 240);
  tft.println("+15");
}

void pressedLeftButoon()
{
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.fillRoundRect(20, 200, 100, 100, 8, ILI9341_GREEN);
  tft.drawRoundRect(20, 200, 100, 100, 8, ILI9341_WHITE);
  tft.setCursor(45, 240);
  tft.println("+15");
}

void resetRightButoon()
{
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.fillRoundRect(120, 200, 100, 100, 8, ILI9341_RED);
  tft.drawRoundRect(120, 200, 100, 100, 8, ILI9341_WHITE);
  tft.setCursor(130, 240);
  tft.println("-800");
}

void pressedRightButoon()
{
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.fillRoundRect(120, 200, 100, 100, 8, ILI9341_GREEN);
  tft.drawRoundRect(120, 200, 100, 100, 8, ILI9341_WHITE);
  tft.setCursor(130, 240);
  tft.println("-800");
}

void background()
{
  tft.fillRoundRect(10, 90, 220, 220, 8, ILI9341_OLIVE);
  tft.drawRoundRect(10, 90, 220, 220, 8, ILI9341_WHITE);
}

void resetBlueButton()
{
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.fillRoundRect(20, 100, 200, 80, 8, ILI9341_BLUE);
  tft.setCursor(90, 115);
  tft.println("TOGGLE");
  tft.setCursor(40, 140);
  tft.println("+15V OR -800V?");
  tft.drawRoundRect(20, 100, 200, 80, 8, ILI9341_WHITE);
  tft.setCursor(40, 165);
  tft.setTextSize(1);
  tft.println("Press this button to reset");
}

void resetBothButtons()
{
  resetLeftButoon();
  resetRightButoon();
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
}

void welcomeText()
{
  tft.setCursor(5, 5);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("...");
  tft.setCursor(40, 25);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(4);
  tft.println("Welcome");
  tft.setCursor(10, 60);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(1);
  tft.println(" Touch the Voltage value to continue");
}

void homeScreen()
{
  tft.fillScreen(ILI9341_BLACK);
  welcomeText();
  background();
  resetBlueButton();
  resetLeftButoon();
  resetRightButoon();
}

void setup()
{
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  delay(1000);
  Serial.begin(115200);
  Serial.print("Demo started");

  tft.begin();

#if (defined ARDUINO_ESP32_WROVER_KIT) || (defined ARDUINO_ESP32_DEV)
  Wire.begin(18, 23);
#else
  Wire.begin();
#endif

  pinMode(TFT_DC, OUTPUT);
  delay(1000);
  testFillScreen();

  homeScreen();
}

void loop(void)
{
  bool answer = Get_yes_no();
  if (answer == true)
  {
    Serial.println("True");
    digitalWrite(RELAY2, LOW);
    delay(1000);
    digitalWrite(RELAY1, HIGH);
  }
  else
  {
    Serial.println("False");
    digitalWrite(RELAY1, LOW);
    delay(1000);
    digitalWrite(RELAY2, HIGH);
  }
}

bool Get_yes_no(void)
{
  while (1)
  {
    TS.ScanBlocking();
    Serial.printf("Screen touched!\n\r");
    if ((TS.Y > 200) && (TS.Y < 300))
    {
      if ((TS.X > 20) && (TS.X < 220))
      {
        if (TS.X > 120)
        {
          resetLeftButoon();
          pressedRightButoon();
          return true;
        }
        else
        {
          resetRightButoon();
          pressedLeftButoon();
          return false;
        }
      }
    }
    else if ((TS.Y > 100) && (TS.Y < 180))
    {
      resetBothButtons();
    }
    while (TS.CheckTouched())
      ;
    Serial.printf("Screen released!\n\r");
  }
}

unsigned long testFillScreen()
{
  unsigned long start = micros();
  tft.fillScreen(ILI9341_RED);
  yield();
  delay(150);
  tft.fillScreen(ILI9341_GREEN);
  yield();
  delay(150);
  tft.fillScreen(ILI9341_BLUE);
  yield();
  delay(150);
  tft.fillScreen(ILI9341_YELLOW);
  yield();
  delay(150);
  return micros() - start;
}
