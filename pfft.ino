#include <Adafruit_ZeroFFT.h>
#include <arm_common_tables.h>

#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/
#define CHANNEL A0
const double res = 7, scale = 10;
const uint16_t samples = 32; //This value MUST ALWAYS be a power of 2
const double samplingFrequency =  res*samples;//Hz, must be less than 10000 due to ADC
double avesum = 0, out;
unsigned int sampling_period_us;
unsigned long microseconds;
int t = 0;
/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double f = 10.5;
double sense = .00000001;
int16_t vReal[samples];
double vImag[samples];
double vdoub[samples];
 const int num = 10;
double vpast[num];
int32_t avg;
double ave, span = 0, gap = 0;
bool start = true;
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void setup()
{
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  Serial.begin(115200);
}

void loop()
{
  /*SAMPLING*/
  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = analogRead(CHANNEL);
      //Serial.println(vReal[i]);
      avg += vReal[i];
      if((abs((vReal[i]-vReal[i-1]))) > (sense))span = micros()-gap;// alter this to be minus average insteadof minus last?
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
  } 

  
  avg = avg/samples;
  for(int i=0; i<samples; i++) vReal[i] = (vReal[i] - avg) * 64;
    
  
  ZeroFFT(vReal, samples);
  /*  if(start = true){
       for(int i = 1; i<num; i++)vpast[i] = vReal[2]*scale/samples;
    start = false; 
  }*/
  avesum = 0;
out = vReal[2];
for(int i = 0; i<num; i++) avesum += vpast[i];
  ave = ((out*scale/samples + avesum)/((num+1)));
  //ave = round(ave);

  Serial.print(50*ave);
  Serial.print(" ");
  Serial.print(span);
  Serial.println('/n');
  gap = micros();
  span = 0;
  vpast[0] = out*scale/samples;
  //for(int i = 0; i<num; i++)Serial.println(vpast[i]);
  for(int i = num-1; i>=0; i--)vpast[i] = vpast[i-1];
 

  vpast[0] = out*scale/samples;
  //for(int i = 0; i<num; i++)Serial.println(vpast[i]);
  
  delay(0); /* Repeat after delay */
}
