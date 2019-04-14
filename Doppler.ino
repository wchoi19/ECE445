#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"

#include <SoftwareSerial.h>

#define SD_ChipSelectPin 53
#define button 25
SoftwareSerial mySerial(19, 18);

// Below: pin number for FOUT
#define PIN_NUMBER 8
// Below: number of samples for averaging
#define AVERAGE 2
// Below: define to use serial output with python script
//#define PYTHON_OUTPUT
// from http://www.limpkin.fr/index.php?post/2013/08/09/Making-the-electronics-for-a-%247-USD-doppler-motion-sensor
unsigned int doppler_div = 19;
unsigned int samples[AVERAGE];
unsigned int x;
TMRpcm tmrpcm;


void setup() {
  tmrpcm.speakerPin=5;
Serial.begin(9600);
 mySerial.begin(38400);
pinMode(PIN_NUMBER, INPUT);
  pinMode(button, INPUT);

if(!SD.begin(SD_ChipSelectPin))
{
  Serial.println("SD fail");
  return;
}
tmrpcm.setVolume(5);
tmrpcm.play("north.wav");
}

void loop() {
noInterrupts();
pulseIn(PIN_NUMBER, HIGH);
unsigned int pulse_length = 0;
for (x = 0; x < AVERAGE; x++)
{
  pulse_length = pulseIn(PIN_NUMBER, HIGH);
  pulse_length += pulseIn(PIN_NUMBER, LOW);
  samples[x] = pulse_length;
}
interrupts();


//button check
 if (digitalRead(button) == HIGH) {
  mySerial.write('1');

 }
 else  {
  mySerial.write('0');
 } 

// Check for consistency
bool samples_ok = true;
unsigned int nbPulsesTime = samples[0];

for (x = 1; x < AVERAGE; x++)
{
nbPulsesTime += samples[x];
if ((samples[x] > samples[0] * 2) || (samples[x] < samples[0] / 2))
{
samples_ok = false;
}
}

if (samples_ok)
{
unsigned int Ttime = nbPulsesTime / AVERAGE;
unsigned int Freq = 1000000 / Ttime;
if(Freq/doppler_div<=120){
if(tmrpcm.isPlaying()==0 && Freq/doppler_div>=7){  
tmrpcm.play("west.wav");
delay(200);
}
#ifdef PYTHON_OUTPUT
Serial.write(Freq/doppler_div);
#else
//Serial.print(Ttime);
Serial.print("\r\n");
Serial.print(Freq);
Serial.print("Hz : ");
Serial.print(Freq/doppler_div);
Serial.print("km/h\r\n");
#endif

delay(200);
}
}
//else
//{
//#ifndef PYTHON_OUTPUT
//Serial.print(".");
//#endif
//}
}
