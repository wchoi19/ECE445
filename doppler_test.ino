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
#define AVERAGE 1
// Below: define to use serial output with python script
//#define PYTHON_OUTPUT
// from http://www.limpkin.fr/index.php?post/2013/08/09/Making-the-electronics-for-a-%247-USD-doppler-motion-sensor
unsigned int doppler_div = 19;
unsigned int samples[AVERAGE];
unsigned int x;

TMRpcm tmrpcm;

typedef struct {

  int pin_number;
  unsigned int samples[AVERAGE];
  bool samples_ok;
  unsigned int nbPulsesTime;
  unsigned int Ttime;
  unsigned int Freq;
 
  
}sensor;


bool zero,one,two,three,four,five;
int max_speed;
int fastest_sensor;

sensor sensor1;

sensor doppler_sensor[6];

PIN_NUMBER[6]={6,7,8,9,10,11};

void setup() {

mrpcm.speakerPin=5;
Serial.begin(9600);
 mySerial.begin(38400);
int j;
for(j=0;j<6; j++){
	//need to change pin number to array
	doppler_sensor[j].pin_number=PIN_NUMBER[j];
	pinMode(doppler_sensor[j].pin_number, INPUT);
}
	max_speed=-1;
if(!SD.begin(SD_ChipSelectPin))
{
  Serial.println("SD fail");
  return;
}
tmrpcm.setVolume(5);
tmrpcm.play("north.wav");

}

void loop() {
int i;
for(i=0; i<6; i++){


//button check
 if (digitalRead(button) == HIGH) {
  mySerial.write('1');

 }
 else  {
  mySerial.write('0');
 } 


noInterrupts();
pulseIn(doppler_sensor[i].pin_number, HIGH);
unsigned int pulse_length = 0;
for (x = 0; x < AVERAGE; x++)
{
pulse_length = pulseIn(doppler_sensor[i].pin_number, HIGH);
pulse_length += pulseIn(doppler_sensor[i].pin_number, LOW);
doppler_sensor[i].samples[x] = pulse_length;
}
interrupts();

// Check for consistency
doppler_sensor[i].samples_ok = true;
doppler_sensor[i].nbPulsesTime = doppler_sensor[i].samples[0];
for (x = 1; x < AVERAGE; x++)
{
doppler_sensor[i].nbPulsesTime += doppler_sensor[i].samples[x];
if ((doppler_sensor[i].samples[x] > doppler_sensor[i].samples[0] * 2) || (doppler_sensor[i].samples[x] < doppler_sensor[i].samples[0] / 2))
{
doppler_sensor[i].samples_ok = false;
}
}

if (doppler_sensor[i].samples_ok)
{
doppler_sensor[i].Ttime = doppler_sensor[i].nbPulsesTime / AVERAGE;
doppler_sensor[i].Freq = 1000000 / doppler_sensor[i].Ttime;

//if it is dangerous
if(doppler_sensor[i].Freq/doppler_div <=100 && doppler_sensor[i].Freq/doppler_div >=15 ){
	
	
	if(doppler_sensor[i].Freq/doppler_div>max_speed){
		max_speed = doppler_sensor[i].Freq/doppler_div;
		fastest_sensor = i;
	}

	if(i==0){
			zero=true;
	}
	else if(i==1){
			one=true;
	}
	else if(i==2){
			two=true;
	}
	else if(i==3){
			three=true;
	}
	else if(i==4){
			four=true;
	}
	else if(i==5){
			five=true;
	}

}

delay(200);
}

}

///			0
///		5		1
///
///		4		2
///			3	
///

	if( tmrpcm.isPlaying()==0){
	
	if(fastest_sensor==0){
			tmrpcm.play("north.wav");
			delay(200);
	}
	else if(fastest_sensor==1){
		if(two==true){
			tmrpcm.play("east.wav");
			delay(200);
		}
		else{
			tmrpcm.play("northeast.wav");
			delay(200);
		}
	}
	else if(fastest_sensor==2){
		if(one==true){
			tmrpcm.play("east.wav");
			delay(200);
		}
		else{
			tmrpcm.play("southeast.wav");
			delay(200);
		}
	}
	else if(fastest_sensor==3){
	
			tmrpcm.play("south.wav");
			delay(200);
		
	}
	else if(fastest_sensor==4){
		if(five==true){
			tmrpcm.play("west.wav");
			delay(200);
		}
		else{
			tmrpcm.play("southwest.wav");
			delay(200);
		}
	}
	else if(fastest_sensor==5){
		if(four==true){
			tmrpcm.play("east.wav");
			delay(200);
		}
		else{
			tmrpcm.play("northeast.wav");
			delay(200);
		}
	}
	
}
	zero=false;
	one=false;
	two=false;
	three=false;
	four=false;
	five=false;
	max_speed=-1;

}
