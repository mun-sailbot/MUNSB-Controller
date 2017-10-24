#include<Wire.h>

int x = 9950;
int xtos(int d);
void xtos2(int d, int length, char words[]);


const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcY=-(Wire.read()<<8|Wire.read());  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcX=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  // Serial.print("AcX = "); Serial.print(AcX);
  // Serial.print(" | AcY = "); Serial.print(AcY);
  // Serial.print(" | AcZ = "); Serial.print(AcZ);
  // Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  // Serial.print(" | GyX = "); Serial.print(GyX);
  // Serial.print(" | GyY = "); Serial.print(GyY);
  // Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(50);
  
  int number = xtos(AcY); // checks how many long the char[] must be
  char stringy[number] = "";
  xtos2(AcY, number, stringy); // sets stringy to represent the number
  //does not handle decimals at this time
  
  Serial.print("X value from Gyro: ");
  Serial.print(stringy);
  Serial.println();
  x++;
}



int xtos(int d){
  int n = 1;
  if(d < 0) {
    d = -d;
    n++;
  }
  bool done = false;
  while (done == false) {
    if (d/10 > 0) {
      d/=10;
      n++;
    } else {
      done = true;
    }
  }
  return n;
}
void xtos2(int d, int length, char words[]) {
  int stop = 0;
  if (d < 0) {
    d = -d;
    words[0] = '-';
    stop = 1;
  }
  for (int i = length-1; i >= stop; i--) {
    words[i] = (char)(d%10+'0');
    d/=10;
  }
}