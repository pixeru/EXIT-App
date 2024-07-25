// Code Writen by Josh
// 3/13/2024

const int PINNUM = 12;

void turnOn() {
  digitalWrite(PINNUM, HIGH);
}

void turnOff() {
  digitalWrite(PINNUM, LOW);
}

void beepBoop() {
  digitalWrite(12, HIGH); // Turn on the LED
  delay(1000); // Wait for 1 second
  digitalWrite(12, LOW); // Turn off the LED
  delay(1000); // Wait for 1 second
}

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud rate
  pinMode(12, OUTPUT); // Set pin 12 as an output
}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    if (inByte == '1') {
      turnOn();
    } else if (inByte == '0') {
      turnOff();
    }
  }
}

// void setup() {
//   pinMode(12, OUTPUT); // Set pin 12 as an output
//   turnOff();
//   //delay(3000);
//   //turnOff();
// }

// // Loop Here
// void loop() {
  
// }