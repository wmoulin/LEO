#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>


SoftwareSerial softSerial(10, 11);// RX, TX

LiquidCrystal_I2C lcd(0x27,20, 4);

long bauds[] = {
    // major 
    9600L, 19200L, 38400L, 57600L, 74880L, 115200L, 230400L, 250000L
  };

void setup()
{



  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("setup !");
  lcd.setCursor(0,1);
  

  if (detectBleBaudRate()) {
    lcd.setCursor(0,2);
    lcd.print("BLE available !!!!!");
  } else {
    lcd.setCursor(0,1);
    lcd.print("BLE not available !");
  }

 /* softSerial.println("AT+NAMELEO");
  delay(500);
  if (softSerial.available()){
    Serial.println("envoie AT+NAMELEO");
    Serial.println("Debut reponse");
    while(softSerial.available()) {
      char c = softSerial.read();
      Serial.print(String(c));
    }
    Serial.println("Fin reponse");
  } else {
    Serial.println("BLE not available !");
  }*/
  //  softSerial.println("AT+NOTI1");
  // --> softSerial.println("AT+FILT0");softSerial.println("AT+SHOW1");
  // softSerial.println("AT+UUID0xBABA");
  // softSerial.println("AT+ADDR"); --> +ADDR=00:15:83:00:56:35
  // softSerial.println("AT+MODE2");
delay(1000);
  if (softSerial.available()){
    Serial.println("BLE available !!!!!");
    Serial.println("Debut reponse");
    while(softSerial.available()) {
      char c = softSerial.read();
      Serial.print(String(c));
    }
    Serial.println("Fin reponse");
  } else {
    Serial.println("No response !");
  }



delay(5000);
}

void loop()
{
  //softSerial.println("AT+MODE?");
  delay(1000);
  if (softSerial.available()){
    Serial.println("BLE available !!!!!");
    Serial.println("Debut reponse");
    while(softSerial.available()) {
      char c = softSerial.read();
      Serial.print(String(c));
    }
    Serial.println("Fin reponse");
  } else {
    Serial.println("No response !");
  }

}

bool detectBleBaudRate() {
  for (int i=0; i<(sizeof(bauds)/sizeof(long)); i++) {
    lcd.setCursor(0,3);

    long cur_baud = bauds[i];
    lcd.print(String(cur_baud));

    softSerial.begin(cur_baud);
  
    delay(500);
    softSerial.println("AT");
    delay(500);

    if (softSerial.available()){
      lcd.print(" : ");
      while(softSerial.available()) {
        char c = softSerial.read();
        lcd.print(String(c));
      }
      
      return true;
    } 
  }
  lcd.setCursor(0,3);
  lcd.print("BLE not available !");
  return false;
}
