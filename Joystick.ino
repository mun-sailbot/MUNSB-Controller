#include <Servo.h>
#include <stdio.h>
Servo myservo;
int pos = 95;
int posMax = 180;
int posMin = 10;    // below this value the servo was pushing and buzzing. (varies with servo)
int xyJoy(char xy);
int deadXMin;
int deadXMax;
int deadYMin;
int deadYMax;
bool goodCal = false;
int xyVal(char xy);

void calibrate();
void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);
  Serial.begin(9600);
  while(goodCal == false){
    calibrate();
  }
  Serial.print("  dXMin: ");Serial.print(deadXMin);
  Serial.print("  dXMax: ");Serial.print(deadXMax);
  Serial.print("  dYMin: ");Serial.print(deadYMin);
  Serial.print("  dYMax: ");Serial.print(deadYMax);
  Serial.println();
  pos = 95;       //need to reiterate pos for some reason, else it sets to 13600.
  myservo.write(pos);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (xyVal('x') > 0 && pos < posMax) {
    if (pos + xyVal('x') < posMax) {
      pos += xyVal('x');
      myservo.write(pos);
    } else {
      myservo.write(posMax);
    }
  } else if (xyVal('x') < 0 && pos > posMin ) {
    if (pos + xyVal('x') > posMin) {
      pos += xyVal('x');
      myservo.write(pos);
    } else {
      myservo.write(posMin);
    }
  }
                  /*  The above section set a servo to maintain position until
                   *  the joystick was pushed off center, and then moved in the
                   *  direction indicated with a speed varying according to the
                   *  magnatude of the offset.
                  */
  
  Serial.print("xJoy: ");Serial.print(xyJoy('x'));
  Serial.print("  x: ");Serial.print(xyVal('x'));
  Serial.print("  pos: ");Serial.println(pos);
  delay(50);
}


int xyJoy(char xy){
                  /*
                   * This function handles x and y components figured I'd just throw
                   * both of their code into the same function.
                   */
  if (xy == 'x') {
    return analogRead(0);//pin 0 is the x axis at this point in time.
  } else if (xy == 'y') {
    return analogRead(1);//pin 1 is the y axis at this point in time.
  } else if (xy == 'p') {
    return analogRead(2);//pin 2 is the push button at this point in time.
  }
  return -1;
}

int xyVal(char xy){
  double factor = 10.0;
  if (xy == 'x') {
    if (xyJoy('x') > deadXMax) {
      return factor*((xyJoy('x') - deadXMax)/(1023.0-deadXMax));
    } else if (xyJoy('x') < deadXMin) {
      return factor*((xyJoy('x')-deadXMin)/(deadXMin-0.0));
    }
  }               /*
                  * factor helped me find an appropriate relation between joystick
                  * offset and motor speed, had to make them ".0" to acheive
                  * non-integer math. Dead refers to the upper and lower
                  * threshholds to ignore in the center of the joystick.
                  */
  return 0;
}



void calibrate(){
  unsigned long timer;
  Serial.println ("Calibrate Dead Zone, wiggle joystick about and click the button");
  Serial.println ("Press button to proceed");
  while ((xyJoy('p') > 20) && (xyJoy('y') > 100)) {
    delay(50);
  }
  
  Serial.println("Beginning calibration...");
  delay(500);
  timer = millis();
  deadXMin = xyJoy('x');
  deadXMax = xyJoy('x');
  deadYMin = xyJoy('y');
  deadYMax = xyJoy('y');
  int deadXMinT = xyJoy('x');
  int deadXMaxT = xyJoy('x');
  int deadYMinT = xyJoy('y');
  int deadYMaxT = xyJoy('y');
  while ((xyJoy('p') > 20) || ( (xyJoy('y') <= 100) && (xyJoy('p') < 20)) ) {
                /*
                 * above is the logic I used to ignore the push button
                 * depnding on the axis state. Took a bit of experimenting to figure out.
                 */
    
    Serial.print("x: ");Serial.print(xyJoy('x'));
    Serial.print("  y: ");Serial.print(xyJoy('y'));
    Serial.print("  p: ");Serial.print(xyJoy('p'));
    Serial.print("  dXMin: ");Serial.print(deadXMin);
    Serial.print("  dXMax: ");Serial.print(deadXMax);
    Serial.print("  dYMin: ");Serial.print(deadYMin);
    Serial.print("  dYMax: ");Serial.print(deadYMax);
    Serial.println();
    if (millis() - timer > 200) {

                /*
                 * Using the above timer, I would only allow .2s old input to affect
                 * the deadzone, this way, if someone unintentionally bumped the joystick
                 * past the intended deadzone while pushgin in on the joystick to confirm, 
                 * any wiggle would not be included in the final reading... unless they
                 * took .2s pushing in which I hope they would not :P
                 */
      
      deadXMin = deadXMinT;
      deadXMax = deadXMaxT;
      deadYMin = deadYMinT;
      deadYMax = deadYMaxT;
      timer = millis();
    }
    if(xyJoy('x') < deadXMinT ){
      deadXMinT = xyJoy('x');
    } else if(xyJoy('x') > deadXMaxT) {
      deadXMaxT = xyJoy('x');
    }
    if(xyJoy('y') < deadYMinT){
      deadYMinT = xyJoy('y');
    } else if(xyJoy('y') > deadYMaxT) {
      deadYMaxT = xyJoy('y');
    }
  }
  char buffer[50];
  int i = 0;
  i=sprintf(buffer,"deadXMin: %d  deadXMax: %d  deadYMin: %d  deadYMax: %d",deadXMin,deadXMax,deadYMin,deadYMax);
  for(int l=0; l<=i; l++)
  Serial.print(buffer[l]);
  Serial.println();
  Serial.println("calibration complete, if you are unsatisfied with the results, pull down, else push up");
  delay(400);

              /*
               * hoping to eventually integrate the above serial into an LCD screen.
               * To be explored later, can use an LED for calibration confirmation and
               * notification.
               */

  
  while ((xyJoy('y') < 800) && (xyJoy('y') > 200)) {
    delay(50);
  }
  if (xyJoy('y') >= 800) {
    Serial.print("Calibration accepted! Running Program");
    goodCal = true;
    deadXMin -= 5;
    deadXMax += 5;
    deadYMin -= 5;
    deadYMax += 5;
    Serial.print("  dXMin: ");Serial.print(deadXMin);
    Serial.print("  dXMax: ");Serial.print(deadXMax);
    Serial.print("  dYMin: ");Serial.print(deadYMin);
    Serial.print("  dYMax: ");Serial.print(deadYMax);
    Serial.println();
  } else {
    goodCal = false;
  }
}

