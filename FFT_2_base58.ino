#include "arduinoFFT.h"

#define SAMPLES 128       //Must be a power of 2
#define SAMPLING_FREQUENCY 2048 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();

const char base58[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
char receivedData[35];
unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];


void setup() {
  Serial.begin(115200);

  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}

void loop() {
  
  /*SAMPLING*/
  for(int i=0; i<SAMPLES; i++)
  {
    microseconds = micros();  //Overflows after around 70 minutes!
  
    vReal[i] = analogRead(0);
    vImag[i] = 0;
  
    while(micros() < (microseconds + sampling_period_us)){
    }
  }

  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);


  int mappedValue = map(peak, 200, 1000, 0, 60);
  int constrainedValue = constrain(mappedValue,0,60);
  
  Serial.print(base58[constrain(mappedValue,0,57)]);

  
  delay(100);  //Repeat the process every second OR:
  //while(1);     //Run code once
}
