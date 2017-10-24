#include <string.h>;
String inputString = "";
void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}


void loop(){
  bool incoming = false;
  while (!Serial){
    //Hold the program here if no serial info available

    /*
      to avoid hangs if it is an issue, could do
      if (!Serial) {
  
      } else {
        and insert all of below into this section.
      }
    */
  };
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if(inChar == '\n') { // detects new lines, signalling end of a serial send
      incoming = true;
      break; // breaks this while loop
    } else {
      inputString += inChar;
    }
  }
  if (incoming == true) {
    Serial.println(inputString); // shows results in serial monitor
    inputString = ""; // reset input string to prep for next msg
  }
}