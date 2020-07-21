// Removes the frequencySweep abstraction from above. This saves memory and
// allows for data to be processed in real time. However, it's more complex.
void frequencySweepRaw(double *MeasuredOutput) {
    // Create variables to hold the impedance data and track frequency
    int real, imag, i = 0, cfreq = START_FREQ/1000;

    // Initialize the frequency sweep
    if (!(AD5933::setPowerMode(POWER_STANDBY) &&         // place in standby
          //AD5933::setControlMode(VOLTAGE_RANGE_4) &&
          AD5933::setControlMode(CTRL_INIT_START_FREQ) && // init start freq
          AD5933::setControlMode(CTRL_START_FREQ_SWEEP))) // begin frequency sweep
         {
             Serial.println("Could not initialize frequency sweep...");
         }

    // Perform the actual sweep
    while ((AD5933::readStatusRegister() & STATUS_SWEEP_DONE) != STATUS_SWEEP_DONE) {
        // Get the frequency data for this frequency point
        if (!AD5933::getComplexData(&real, &imag)) {
            Serial.println("Could not get raw frequency data...");
        }

        // Print out the frequency data
//        Serial.print(cfreq);
//        Serial.print(": R=");
//        Serial.print(real);
//        Serial.print("/I=");
//        Serial.print(imag);

        // Compute impedance
        double magnitude = sqrt(pow(real, 2) + pow(imag, 2));
        //double impedance = 1/(magnitude*Gain[i]);
        MeasuredOutput[i] = magnitude;
//        Serial.print("  |Z|=");
//        Serial.println(impedance);
//        mySerial.print(cfreq);
//        mySerial.print(",");
//        mySerial.print(real);
//        mySerial.print(",");
//        mySerial.print(imag);
//        mySerial.print(",");  
//        mySerial.println(impedance);
//        Serial.println("SoftwarSerialSent");
      
        // Increment the frequency
        i++;
        cfreq += FREQ_INCR/1000;
        AD5933::setControlMode(CTRL_INCREMENT_FREQ);
        //delay(200);
    }

    Serial.println("Frequency test complete!");

    // Set AD5933 power mode to standby when finished
    if (!AD5933::setPowerMode(POWER_STANDBY))
        Serial.println("Could not set to standby...");
}
