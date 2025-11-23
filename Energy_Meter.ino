const int sensorIn = A0;

// Sensitivity (mV per Amp) 
int mVperAmp = 185;  // 5A = 185, 20A = 100, 30A = 66

double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double power = 0;
unsigned long readingIndex = 1;

const float ADC_REF = 5.0;  // 5V Arduino

void setup() {
  Serial.begin(9600);
}

void loop() {
  Voltage = getVPP();
  VRMS = (Voltage / 2.0) * 0.707;       // Vpp → Vpeak → Vrms
  AmpsRMS = (VRMS * 1000.0) / mVperAmp; // Vrms (V) → mV → A
  power = 230.0 * AmpsRMS;              // Apparent power, assuming 230V & PF=1

  Serial.print("\n\nReading no: ");
  Serial.println(readingIndex++);
  
  Serial.print("Amps RMS: ");
  Serial.println(AmpsRMS, 3);

  Serial.print("Voltage RMS (sensor): ");
  Serial.println(VRMS, 3);

  Serial.print("Power (approx, VA/W): ");
  Serial.println(power, 2);
}

float getVPP() {
  int readValue;
  int maxValue = 0;
  int minValue = 1023;

  unsigned long start_time = millis();
  while ((millis() - start_time) < 950) { // sample for ~0.95 s
    readValue = analogRead(sensorIn);
    if (readValue > maxValue) maxValue = readValue;
    if (readValue < minValue) minValue = readValue;
  }

  float Vpp = (maxValue - minValue) * ADC_REF / 1024.0;
  return Vpp;
}
