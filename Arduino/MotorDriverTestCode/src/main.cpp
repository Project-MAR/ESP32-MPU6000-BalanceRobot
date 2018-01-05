#include <Arduino.h>

#define LED_BUILTIN 2

int channelA = 0;
int channelB = 1;
int channe2A = 2;
int channe2B = 3;

int freq = 5000;
int resolution = 8;

int Motor1A = 25;
int Motor1B = 33;

int Motor2A = 27;
int Motor2B = 26;

void setup() 
{
  Serial.begin(115200);
  ledcSetup(channelA, freq, resolution);    // Init PWM Frequency and PWM Resolution to PWM Channel 0
  ledcAttachPin(Motor1A, channelA);         // Attach PWM Channel 0 to ESP32 Pin 25
  //ledcAttachPin(LED_BUILTIN, channelA);   // Test PWM with build-in LED
  
  ledcSetup(channelB, freq, resolution);    // Init PWM Frequency and PWM Resolution to PWM Channel 1
  ledcAttachPin(Motor1B, channelB);         // Attach PWM Channel 1 to ESP32 Pin 33

  ledcSetup(channe2A, freq, resolution);    // Init PWM Frequency and PWM Resolution to PWM Channel 2
  ledcAttachPin(Motor2A, channe2A);         // Attach PWM Channel 2 to ESP32 Pin 27
  
  ledcSetup(channe2B, freq, resolution);    // Init PWM Frequency and PWM Resolution to PWM Channel 3
  ledcAttachPin(Motor2B, channe2B);         // Attach PWM Channel 3 to ESP32 Pin 26
}

void loop() 
{ 
  // Forward Direction
  ledcWrite(channelA, 0);                   // Set PWM Duty Cycle
  ledcWrite(channe2A, 0);                   // Set PWM Duty Cycle
 
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 10)
  {
    Serial.println(dutyCycle);
    ledcWrite(channelB, dutyCycle);
    ledcWrite(channe2B, dutyCycle);
    delay(500);
  }
  
  // Reverse Direction
  ledcWrite(channelB, 0);
  ledcWrite(channe2B, 0);

  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 10)
  {
    Serial.println(dutyCycle);
    ledcWrite(channelA, dutyCycle);
    ledcWrite(channe2A, dutyCycle);
    delay(500);
  }

}