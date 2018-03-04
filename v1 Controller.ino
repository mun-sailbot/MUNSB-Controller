    
void rudderControl(); // Declaration
  
  //Rudder variables
  int rudderPOT = A0;
  int rudderTemp = 0;
  int rudderValue = 0;

  
  //Mast variables
  //int mastPOT = A1;
  int mastValue = 0;
  int mastTemp = 0;

  //Digital switchs for autonomy functions
  int autonemy = 9; //sets autonemy switch to pin 9;
  
      //add variables for each event here
  
void setup() 
{
  pinMode (autonemy, INPUT);
  
  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  while (!Serial) 
  {
    // wait for serial port to connect.
  }
  
  // Read initial value of rudderPOT 
  rudderValue =  analogRead(rudderPOT); 
  rudderTemp = rudderValue;
  
  //Send identifier char followed by POT value 
  Serial.print('R');
  Serial.print(rudderValue);
  
  // Read initial value of mastPOT 
  //mastValue = analogRead(mastPOT);
  //mastTemp = mastValue;
  
  //Send identifier char followed by POT value 
  //Serial.print('M');
  //Serial.print(mastValue);
}

void loop() 
{
  int test = digitalRead(autonemy);
  
  while(test == LOW)
  {
      rudderControl();
      test = digitalRead(autonemy);   
  }
  
  Serial.print("autonemy");
  
}


void rudderControl()
{
  // Read value of rudderPOT 
    rudderValue =  analogRead(rudderPOT);
  
    if ((rudderValue != rudderTemp) and (rudderValue != rudderTemp+1) and (rudderValue != rudderTemp-1))
    {
    //Send identifier char followed by POT value 
    Serial.print('R');
    Serial.print(rudderValue);
    rudderTemp = rudderValue;
    }
}

void mastControl()
{
  // Read value of mastPOT 
  mastValue =  analogRead(mastPOT);
  
  if ((mastValue != mastTemp) and (mastValue != mastTemp+1) and (mastValue != mastTemp-1))
  {
  //Send identifier char followed by POT value 
  Serial.print('M');
  Serial.print(mastValue);
  mastTemp = mastValue;
  }
}
