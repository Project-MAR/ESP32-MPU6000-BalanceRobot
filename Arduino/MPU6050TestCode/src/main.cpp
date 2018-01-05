#include <Arduino.h>
#include <Wire.h>

// Original Code From
// https://www.controleverything.com/content/Accelorometer?sku=MPU-6000_I2CS#tabs-0-product_tabset-2
// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MPU-60x0


// For MPU-6000, I2C address is 0x68 When AD = 0, otherwise 0x69
#define Addr 0x68
#define LED_BUILTIN 2

#define MPU_RAW        0    // 2's Complement format but print like decimal format
#define MPU_DECIMAL    1    // 2's Complement format that convert to decimal format
#define MPU_REALDATA   2    // decimal format that convert to actual data

unsigned int data[6];
int xAccl  = 0;
int yAccl  = 0;
int zAccl  = 0;
int xGyro  = 0;
int yGyro  = 0;
int zGyro  = 0;

// Original Code From http://forum.arduino.cc/index.php/topic,44216.0.html#13
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)
void printDouble(double val, unsigned int precision)
{
   Serial.print (int(val));   //prints the int part
   Serial.print(".");         // print the decimal point
   unsigned int frac;
   if(val >= 0)
     frac = (val - int(val)) * precision;
   else
      frac = (int(val)- val ) * precision;
   int frac1 = frac;
   while( frac1 /= 10 )
       precision /= 10;
   precision /= 10;
   while(  precision /= 10)
       Serial.print("0");

   Serial.println(frac,DEC) ;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);             // Initialise serial communication, set baud rate = 115200
  
  Wire.begin();                     // Initialise I2C communication as Master
  Wire.setClock(400000);            // choose 400 kHz I2C rate

  Wire.beginTransmission(Addr);     // Start I2C transmission
  Wire.write(0x1B);                 // Select gyroscope configuration register
  //Wire.write(0x18);               // Full scale range = +/-2000 dps
  //Wire.write(0x08);               // Full scale range = +/-500 dps
  Wire.write(0x00);                 // Scale range = +/-250 dps
  Wire.endTransmission();           // Stop I2C transmission

  Wire.beginTransmission(Addr);     // Start I2C transmission
  Wire.write(0x1C);                 // Select accelerometer configuration register
  //Wire.write(0x18);               // Full scale range = +/-16g
  //Wire.write(0x08);               // Full scale range = +/-4g
  Wire.write(0x00);                 // Scale range = +/-2g
  Wire.endTransmission();           // Stop I2C transmission

  Wire.beginTransmission(Addr);     // Start I2C transmission
  Wire.write(0x6B);                 // Select power management register
  Wire.write(0x01);                 // PLL with xGyro reference
  Wire.endTransmission();           // Stop I2C transmission
  delay(500);
}

int TwoComplementToNormalValue(int data)
{
  unsigned char raw[2];
  int result = 0;
  
  //Serial.print("2's complement value: ");
  //Serial.println(data);
  //Serial.print("minus check: ");
  //Serial.println(data & 0x8000);

  if((data & 0x8000) != 0)
  {
    //Serial.println("Detect Negative Value");
    data = data - 1;
    raw[0] = data >> 8;
    raw[1] = data & 0xFF;
    
    for(int i = 0; i<=1; i++)
       raw[i] ^= 0xFF;

    result = (raw[0] << 8) + raw[1];
    result = -result;
  }
  else
  {
    //Serial.println("Detect Positive Value");
    result = data;
  }
  
  //Serial.print("Decimal Value: ");
  //Serial.println(result);
  //while(true){ };
  
  return result;
}

void printMPU6000(int mode)
{
  if(mode == MPU_RAW)
  {
    Serial.println("");
    Serial.print("Acceleration in X-Axis : ");
    Serial.println(xAccl);
    Serial.print("Acceleration in Y-Axis : ");
    Serial.println(yAccl);
    Serial.print("Acceleration in Z-Axis : ");
    Serial.println(zAccl);
    Serial.print("X-Axis of Rotation : ");
    Serial.println(xGyro);
    Serial.print("Y-Axis of Rotation : ");
    Serial.println(yGyro);
    Serial.print("Z-Axis of Rotation : ");
    Serial.println(zGyro);
  }
  else if (mode == MPU_DECIMAL)
  {
    Serial.println("");
    Serial.print("Acceleration in X-Axis : ");
    Serial.println(TwoComplementToNormalValue(xAccl));
    Serial.print("Acceleration in Y-Axis : ");
    Serial.println(TwoComplementToNormalValue(yAccl));
    Serial.print("Acceleration in Z-Axis : ");
    Serial.println(TwoComplementToNormalValue(zAccl));
    Serial.print("X-Axis of Rotation : ");
    Serial.println(TwoComplementToNormalValue(xGyro));
    Serial.print("Y-Axis of Rotation : ");
    Serial.println(TwoComplementToNormalValue(yGyro));
    Serial.print("Z-Axis of Rotation : ");
    Serial.println(TwoComplementToNormalValue(zGyro));
  }
  else if(mode == MPU_REALDATA)
  {
    /* Not Working, DO NOT USE */
    Serial.println("");
 
    float Real_Accl_X = (((32768.0 + (float)TwoComplementToNormalValue(xAccl)) * 4.0) / 65535.0) - 2.0;
    
    /*
    int yAccl = TwoComplementToNormalValue(yAccl);
    int zAccl = TwoComplementToNormalValue(zAccl);

    int xGyro = TwoComplementToNormalValue(xGyro);
    int yGyro = TwoComplementToNormalValue(yGyro);
    int zGyro = TwoComplementToNormalValue(zGyro);
    */

    Serial.print("Acceleration in X-Axis : ");
    Serial.print(TwoComplementToNormalValue(xAccl));
    Serial.print(" => ");
    printDouble(Real_Accl_X, 10000000);
    /*
    Serial.print("Acceleration in X-Axis : ");
    printDouble(TwoComplementToNormalValue(zGyro), 10000000);
    Serial.print("Acceleration in Y-Axis : ");
    printDouble(TwoComplementToNormalValue(yAccl), 10000000);
    Serial.print("Acceleration in Z-Axis : ");
    printDouble(TwoComplementToNormalValue(zAccl), 10000000);
    Serial.print("X-Axis of Rotation : ");
    printDouble(TwoComplementToNormalValue(xGyro), 10000000);
    Serial.print("Y-Axis of Rotation : ");
    printDouble(TwoComplementToNormalValue(yGyro), 10000000);
    Serial.print("Z-Axis of Rotation : ");
    printDouble(TwoComplementToNormalValue(zGyro), 10000000);
    */
  }
}

int status = 0;
void loop()
{
  //TwoComplementToNormalValue(65471);    // 65471 == -65

  if(status == 1)
  {
    status = 0;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    status = 1;
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  Wire.beginTransmission(Addr);   // Start I2C transmission
  Wire.write(0x3B);               // Select data register
  Wire.endTransmission();         // Stop I2C transmission
  Wire.requestFrom(Addr, 6);      // Request 6 bytes of data
  
  // Read 6 byte of data 
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read(); 
  }
  
  xAccl = (data[0]<<8) + data[1];
  yAccl = (data[2]<<8) + data[3];
  zAccl = (data[4]<<8) + data[5];

  Wire.beginTransmission(Addr);   // Start I2C transmission
  Wire.write(0x43);               // Select data register 
  Wire.endTransmission();         // Stop I2C transmission
  Wire.requestFrom(Addr, 6);      // Request 6 bytes of data
  
  // Read 6 byte of data 
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read(); 
  }

  xGyro = (data[0]<<8) + data[1];
  yGyro = (data[2]<<8) + data[3];
  zGyro = (data[4]<<8) + data[5];

  //printMPU6000(MPU_RAW);
  printMPU6000(MPU_DECIMAL);
  //printMPU6000(MPU_REALDATA);
  delay(1000);

}