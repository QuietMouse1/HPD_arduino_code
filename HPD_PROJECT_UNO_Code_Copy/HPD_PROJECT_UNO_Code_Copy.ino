/*
ad5933-test
    Reads impedance values from the AD5933 over I2C and prints them serially.
*/


#include <Wire.h>
#include <AD5933.h>
#include <SoftwareSerial.h>

#define START_FREQ       (10000)
#define FREQ_INCR        (10000)
#define NUM_INCR         (20)
#define REF_RESIST       (49.75)


double gain[NUM_INCR+1];
int phase[NUM_INCR+1];

float impedance[NUM_INCR];
float frequency[NUM_INCR];
float magnitude[NUM_INCR];
//nSoftwareSerial mySerial(2, 3); // RX, TX

int pin4 = 4;
int rstEsp_pin = 5;

double measuredVoltage[NUM_INCR];
double measuredCurrent[NUM_INCR];


void setup()
{
  pinMode(pin4, OUTPUT);
  Serial.begin(9600);
  //mySerial.begin(9600);

  // Begin I2C
  Wire.begin();
  delay(2000);
  // Begin serial at 9600 baud for output
  Serial.println("AD5933 Test Started!");
  // Perform initial configuration. Fail if any one of these fail.
    if (!(AD5933::reset()))
    {
      Serial.println("Failed in reset");
    }
    if (!AD5933::setInternalClock(true))
    {
      Serial.println("Failed in Clock");
    }
    if (!AD5933::setStartFrequency(START_FREQ))
    {
      Serial.println("Failed in settingStartFreq");
    }
    if (!AD5933::setIncrementFrequency(FREQ_INCR))
    {
      Serial.println("Failed in FRE INCR");
    }
    if (!AD5933::setNumberIncrements(NUM_INCR))
    {
      Serial.println("Failed in NUM_INCR");
    }
    if (!AD5933::setPGAGain(PGA_GAIN_X1))
    {
      Serial.println("Failed in PGA GAIN X1");
    }
    if(!AD5933::setControlMode(VOLTAGE_RANGE_2))
    {
      Serial.println("FAILED TO ADJUST VOLTAGE RANGE");
    }

  for (int i = 0; i < NUM_INCR + 1; i++) {
    gain[i] = 0.0;
    phase[i] = 0;
  }
  // Perform calibration sweep
  if (calibrateNew(gain, phase, REF_RESIST, NUM_INCR+1)){
    Serial.println("Calibrated!");
  }
  else{
    Serial.println("Calibration failed...");
  }
  delay(100); //Allow for serial print to complete.
}

String inString = "";
void loop(){
  //Wait for esp to send command for calibration data
  while(!Serial.available());
  while(Serial.available()){
    String command = Serial.readString();
    String dataToEsp = "";
    
    if (command == "Calibrate"){
      calibrateNew(gain, phase, REF_RESIST, NUM_INCR+1);
      String dataToEsp = "NanoCalibration=";
      for (int i = 0; i < NUM_INCR; i ++){
        dataToEsp += String(gain[i]);
      }
      Serial.println(dataToEsp);
    }
    
    if (command == "Sweep"){
      digitalWrite(pin4,LOW); // measure voltage
      frequencySweepRaw(measuredVoltage);
      digitalWrite(pin4,HIGH); // measure current
      frequencySweepRaw(measuredCurrent);
      dataToEsp="NanoSweep=";
      for (int i = 0 ; i < NUM_INCR; i++){
        double measuredImpedance = measuredVoltage[i]/measuredCurrent[i]*gain[i];
        dataToEsp += String(measuredImpedance);
        if ( i != NUM_INCR){
          dataToEsp += ';';
        }
      }
      Serial.println(dataToEsp);
    }
    
  }
}
