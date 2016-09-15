#include <SoftwareSerial.h> 

SoftwareSerial bluetooth(10, 11);// RX, TX

int RPIN = 5;  // what PIN are you using for RED?
int BPIN = 6; // what PIN are you using for BLUE?
int GPIN = 9; // what PIN are you using for GREEN?

String header = "186;186;170;170;";
String data = "";
String separator = ";";
String cmdCode = "";
String cmdValues = "";
String cmdValue = "";
int r = 0;
int g = 0;
int b = 0;

long bauds[] = { 
    // major 
    9600L, 19200L, 38400L, 57600L, 74880L, 115200L, 230400L, 250000L 
  };


void setup() 
{ 
  Serial.begin(9600L);
  bluetooth.begin(9600L);

  setAllLedlight(255, 0, 0);
  delay(2000);

  setAllLedlight(0, 255, 0);
  delay(2000);

  setAllLedlight(0, 0, 255);
  delay(2000);

  setAllLedlight(0, 0, 0);

} 

void loop() 
{ 
  if(bluetooth.available()) { 
      Serial.print("bluetooth write : "); 
      data = "";
      delay(100);
      while(bluetooth.available()) 
      { 
        byte toSend = (byte)bluetooth.read(); 
        Serial.print((char)toSend);
        data.concat(String(toSend, DEC));
        data.concat(separator);
      } 
      Serial.println(""); 
      Serial.println(data);
      extractCommand();
      execCommand();
   }

  //Read from usb serial to bluetooth 
  if(Serial.available()) { 
    Serial.print("Serial write : "); 
    while(Serial.available()) 
    { 
      char toSend = (char)Serial.read(); 
      bluetooth.write(toSend); 
      Serial.print(toSend); 
    }
    bluetooth.println("");
    Serial.println("");
  }



  
  //Read from usb serial to bluetooth 
  /*if(Serial.available()) {
    lcd.setCursor(0,0);
    lcd.print("read : ");

    while(Serial.available()) 
    { 
      byte toSend = (byte)Serial.read(); 
      lcd.print(toSend, HEX);  
    }
  } */
} 

/*bool detectBleBaudRate() { 
  for (int i=0; i<(sizeof(bauds)/sizeof(long)); i++) { 
    long cur_baud = bauds[i]; 
    bluetooth.begin(cur_baud); 
  
    delay(500); 
    bluetooth.println("AT"); 
    delay(500); 

    if (bluetooth.available()){ 
      while(bluetooth.available()) { 
        char c = bluetooth.read(); 
        Serial.print(String(c)); 
      } 
      
      return true; 
    } 
  } s
  Serial.println("BLE not available !"); 
  return false; 
}*/

void setAllLedlight(int redValue, int greenValue, int blueValue) {
  setLedlight(RPIN, redValue);
  setLedlight(GPIN, greenValue);
  setLedlight(BPIN, blueValue);
}

void setLedlight(int pin, int value) {
  analogWrite(pin,map(value, 0, 255, 0, 1023));
}

void execCommand() {
  switch (cmdCode.toInt()) {
    case 3:
     r = nextCommandValue();
      g = nextCommandValue();
      b = nextCommandValue();
      setAllLedlight(r, g, b);
      break;
    case 2:
      //do something when var equals 2
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
  
}


void extractCommand() {
  if (data.startsWith(header)) {
    String cmd = data.substring(data.indexOf(header) + header.length());
    cmdCode = cmd.substring(0, cmd.indexOf(separator));
    cmdValues = cmd.substring(cmd.indexOf(separator) + 1);
  }
}

int nextCommandValue() {
  cmdValue = cmdValues.substring(0, cmdValues.indexOf(separator));
  cmdValues = cmdValues.substring(cmdValues.indexOf(separator) + 1);
  return cmdValue.toInt();
}

