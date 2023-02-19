/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
void loop() {
  if (irrecv.decode()) {
    unsigned long result = irrecv.decodedIRData.decodedRawData;
    // Serial.println(result, HEX);
    irrecv.resume(); // Receive the next value
  
  switch (result)
  {
  case 0xF8070707:
    Serial.println("V+");
    break;

  case 0xF40B0707:
    Serial.println("V-");
    break;

  case 0xED120707:
    Serial.println("P+");
    break;
  case 0xEF100707:
    Serial.println("P-");
    break;

  case 0xFB040707:
    Serial.println("1");
    break;

  case 0xFA050707:
    Serial.println("2");
    break;

  case 0xF9060707:
    Serial.println("3");
    break;

  case 0xF7080707:
    Serial.println("4");
    break;

  case 0xF6090707:
    Serial.println("5");
    break;

  case 0xF50A0707:
    Serial.println("6");
    break;

  case 0xF30C0707:
    Serial.println("7");
    break;

  case 0xF20D0707:
    Serial.println("8");
    break;

  case 0xF10E0707:
    Serial.println("9");
    break;
  default:
    break;
  }
  }
}