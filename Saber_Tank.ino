#include <SoftwareSerial.h>
#include <Sabertooth.h>
#include <XBee.h>

XBee xbee = XBee();

Rx16IoSampleResponse io16 = Rx16IoSampleResponse();
Rx64IoSampleResponse io64 = Rx64IoSampleResponse();

SoftwareSerial SerialAlt(NOT_A_PIN, 10);

Sabertooth ST(128, SerialAlt);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  SerialAlt.begin(9600);
  
  xbee.setSerial(Serial1);
  
  ST.autobaud();
}

void loop() {
  XBeeResponse response;
  
  xbee.readPacket();
  
  response = xbee.getResponse();
  if (response.isAvailable()) {
    if (response.getApiId() == RX_16_IO_RESPONSE) {
      response.getRx16IoSampleResponse(io16);
      setMotors(1024 - io16.getAnalog(0,0), io16.getAnalog(2,0));
    } else if (response.getApiId() == RX_64_IO_RESPONSE) {
      response.getRx64IoSampleResponse(io64);
      setMotors(1024 - io64.getAnalog(0,0), io64.getAnalog(2,0));
    } else {
      Serial.print("API Packet: ");
      Serial.println(response.getApiId(), HEX);
    }
  }
}

void setMotors(int a, int b) {
  Serial.print(a, DEC);
  Serial.print(", ");
  Serial.println(b, DEC);
  ST.motor(1, a - 512);
  ST.motor(2, b - 512);
}
