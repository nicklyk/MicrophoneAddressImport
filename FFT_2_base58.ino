#include "arduinoFFT.h"

#define SAMPLES 128       //Must be a power of 2
#define SAMPLING_FREQUENCY 2048 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();

const char base58[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
char key[35];
char runningList[5];
int counter = 0;
int counter_2 = 0;
bool record;
bool printable = false;

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[SAMPLES];
double vImag[SAMPLES];


void setup() {
  Serial.begin(115200);

  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}

  /*Main Loop*/
void loop(){
  
  int mappedPeak = map(FFTcomp(), 200, 1000, 0, 60);  //Maps the peak to a value ranging from 0-60
  Serial.print(base58[constrain(mappedPeak,0,57)]);   //Uses the mapped peak as an index, to print from the base58 array

  runningList[counter] = base58[constrain(mappedPeak,0,57)];  //An array with a looping index 'counter'(0-4), used in start checking
  //delay(80);


  /*Checks if START was played, starts recording the next 35 characters into key[]*/
  if(counter == 4 && runningList[0] == 'S' && runningList[1] == 'T' && runningList[2] == 'A' && runningList[3] == 'R' && runningList[4] == 'T'){
  /*start recording and saving*/
    Serial.print(" Recording ");
    while(counter_2 <= 34){
      mappedPeak = map(FFTcomp(), 200, 1000, 0, 60);
      key[counter_2] = base58[constrain(mappedPeak,0,57)];
      Serial.print(key[counter_2]);
      counter_2 += 1;
      }
    printable = true;
  }

  if(counter <4){counter+=1;}else{counter=0;counter_2=0;}
  if(printable){for(int y=0;y<=34;y++){Serial.print(key[y]);}Serial.print(" END OF RECORDING ");printable=false;}


}


double FFTcomp() {
  
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
  return peak;
  
}
