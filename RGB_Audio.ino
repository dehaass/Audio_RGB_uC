 
int analogPin = A0; // MSGEQ7 OUT
int strobePin = A1; // MSGEQ7 STROBE
int resetPin = A2; // MSGEQ7 RESET
int spectrumValue[7];
 
// MSGEQ7 OUT pin produces values around 50-80
// when there is no input, so use this value to
// filter out a lot of the chaff.
int filterValue = 80;
 
// LED pins connected to the PWM pins on the Arduino
 
int ledPinR = 3;
int ledPinG = 6;
int ledPinB = 5;
int ledPinW = 9;
 
void setup()
{
  Serial.begin(115200);
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
 
  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}
 
void loop()
{
  // Set reset pin low to enable strobe
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
 
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
 
    // Remove serial stuff after debugging
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
    digitalWrite(strobePin, HIGH);
   }
 
   Serial.println();
 
   // Write the PWM values to the LEDs
   // I find that with three LEDs, these three spectrum values work the best
   analogWrite(ledPinR, spectrumValue[1]);
   analogWrite(ledPinG, spectrumValue[4]);
   analogWrite(ledPinB, spectrumValue[6]);
   analogWrite(ledPinW, spectrumValue[1]);
}
