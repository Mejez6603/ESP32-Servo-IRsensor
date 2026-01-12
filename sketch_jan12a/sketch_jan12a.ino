#include <Arduino.h>
#include <IRremote.hpp>
#include <ESP32Servo.h>

// Pin Definitions (Updated to your latest pins)
const int IR_RECEIVE_PIN = 35; 
const int LED_PIN = 32;
const int SERVO_A_PIN = 13;
const int SERVO_B_PIN = 12; 
const int SERVO_C_PIN = 14;
const int SERVO_D_PIN = 27;

Servo servoA, servoB, servoC, servoD;
uint16_t lastCommand = 0; 

void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  pinMode(LED_PIN, OUTPUT);
  
  servoA.attach(SERVO_A_PIN);
  servoB.attach(SERVO_B_PIN);
  servoC.attach(SERVO_C_PIN);
  servoD.attach(SERVO_D_PIN);
  
  stopServos(); 
  Serial.println("System Ready - Staggered Toggle Enabled");
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t currentCommand = IrReceiver.decodedIRData.command;
    digitalWrite(LED_PIN, HIGH);

    if (currentCommand == lastCommand) {
      Serial.println("Toggle: Returning to Default");
      
      if (currentCommand == 0x12) {
        // Special staggered reset for Button 4
        resetButton4Sequence();
      } else {
        // Normal reset for other buttons
        stopServos();
      }
      lastCommand = 0; 
    } else {
      handleMovement(currentCommand);
      lastCommand = currentCommand; 
    }

    delay(200); 
    digitalWrite(LED_PIN, LOW);
    IrReceiver.resume(); 
  }
}

void stopServos() {
  servoA.write(90);
  servoB.write(90);
  servoC.write(90);
  servoD.write(90);
}

// The staggered sequence you requested
void resetButton4Sequence() {
  Serial.println("Staggered Reset: CD first, then AB");
  
  // 1. Move C and D to default (90)
  servoB.write(90);
  servoD.write(90);
  
  // 2. Wait for CD to reach position (approx 500ms for 90 degrees)
  delay(500); 
  
  // 3. Move A and B to default (90)
  servoA.write(90);
  servoC.write(90);
}

void handleMovement(uint16_t cmd) {
  switch (cmd) {
    case 0xE: // Button 1
      servoA.write(0);
      servoC.write(180);
      break;

    case 0x6: // Button 2
      servoB.write(0);
      servoD.write(180);
      break;

    case 0xF: // Button 3
      servoA.write(0);
      servoB.write(0);
      servoC.write(180);
      servoD.write(180);
      break;

    case 0x12: // Button 4 - Your new sequence
      Serial.println("Action: Button 4 Activated");
      servoA.write(0);
      servoB.write(0);
      servoC.write(180);
      servoD.write(180);
      break;

    default:
      stopServos();
      lastCommand = 0;
      break;
  }
}