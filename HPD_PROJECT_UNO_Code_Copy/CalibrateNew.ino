bool calibrateNew(double gain[], int phase[], double ref, int n) {
  for (int x = 0; x < 1; x++) {
    // We need arrays to hold the real and imaginary values temporarily
    int *realV = new int[n];
    int *imagV = new int[n];
    int *realI = new int[n];
    int *imagI = new int[n];

    digitalWrite(pin4, LOW);
    // Perform the frequency sweep
    if (!AD5933::frequencySweep(realV, imagV, n)) {
        delete [] realV;
        delete [] imagV;
        return false;
        
    }

    digitalWrite(pin4, HIGH);
    if (!AD5933::frequencySweep(realI, imagI, n)) {
        delete [] realI;
        delete [] imagI;
        return false;
    }

    // For each point in the sweep, calculate the gain factor and phase
    for (int i = 0; i < n; i++) {
      gain[i] += (ref) * sqrt(pow(realI[i], 2) + pow(imagI[i], 2)) / sqrt(pow(realV[i], 2) + pow(imagV[i], 2));
    }


    delete [] realV;
    delete [] imagV;
    delete [] realI;
    delete [] imagI;
  }

  for (int i = 0; i < n; i++) {
    gain[i] = ((double) gain[i]) / 1.0;
  }
  
  return true;
}
