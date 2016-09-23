#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#define MODE_LED_BEHAVIOUR          "MODE"
#define FACTORYRESET_ENABLE         0

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);




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




void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command <-> Data Mode Example"));
  Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
  ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);


  // Set module to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));

  setAllLedlight(255, 0, 0);
  delay(2000);

  setAllLedlight(0, 255, 0);
  delay(2000);

  setAllLedlight(0, 0, 255);
  delay(2000);

  setAllLedlight(0, 0, 0);
}


void loop(void)
{
  // Check for user input
  char n, inputs[BUFSIZE+1];

  if (Serial.available())
  {
    n = Serial.readBytes(inputs, BUFSIZE);
    inputs[n] = 0;
    // Send characters to Bluefruit
    Serial.print("Sending: ");
    Serial.println(inputs);

    // Send input data to host via Bluefruit
    ble.print(inputs);
  }

  // Echo received data
  if ( ble.available() )
  {
    Serial.print("bluetooth write : "); 
    data = "";
    delay(100);
    while ( ble.available() )
    {
      byte toSend = (byte)ble.read();
      Serial.print((char)toSend);
      data.concat(String(toSend, DEC));
      data.concat(separator);
    }
    extractCommand();
    execCommand();
  }
}
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

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
