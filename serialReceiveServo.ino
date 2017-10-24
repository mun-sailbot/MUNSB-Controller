// int incomingByte = 0;   // for incoming serial data
//16384
#include <string.h>
#include <Servo.h>
using namespace std;

Servo myservo;  // create servo object to control a servo
String inputString = "";
int val = (20+175)/2;    // variable to read the value from the analog pin
int serLoc = 0;
int xRot = 0;
void updateServo();

void setup() {
  myservo.attach(9);
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop(){
  // send data only when you receive data:
  bool incoming = false;
  while (!Serial){
    // Not even sure if this is necesary but will work on that later
  };
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if(inChar == '\n') {
      incoming = true;
      break;
    } else {
      inputString += inChar;
    }
  }
  if (incoming == true) {
    Serial.println(inputString);
    // Serial.println(inputString.length());
    // Serial.println(inputString.indexOf("xRot", 0));
    // Serial.println(inputString.indexOf(" ", 0));
    // Serial.println(inputString.substring(inputString.indexOf(" ", 0) +1, (inputString.length() -1)));
    xRot = inputString.substring(inputString.indexOf(" ", 0) +1, (inputString.length() -1)).toInt();
    Serial.println(xRot);
    inputString = "";
    serLoc = map(xRot, -16384, 16384, 20, 175);
    Serial.print("\t");Serial.print(serLoc);Serial.println();
    myservo.write(serLoc);
  }
}