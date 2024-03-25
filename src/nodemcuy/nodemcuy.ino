#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SPI.h>

#define SDA_PIN D2
#define SCL_PIN D1
#define RST_PIN D3
#define SS_PIN D4
#define LED_PIN D8

MFRC522 mfrc522(SS_PIN, RST_PIN);  
LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long lastScanTime = 0;
bool isCardPresent = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.begin();
  lcd.backlight();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 

  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  if (readRFID()) {
    if (!isCardPresent || (millis() - lastScanTime > 6000)) {
      displayWelcomeMessage();
      digitalWrite(LED_PIN, HIGH);
      delay(500);  
      digitalWrite(LED_PIN, LOW); 
      lcd.clear();
      lastScanTime = millis();
      isCardPresent = true;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Anda sudah absen");
      delay(3000);
      lcd.clear();
    }
  } else {
    isCardPresent = false;
  }
}

bool readRFID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    return true;
  } else {
    return false;
  }
}

void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ohayou!");
  lcd.setCursor(0, 1);
  lcd.print("oni-chan >w<");  
  delay(3000);  
}
