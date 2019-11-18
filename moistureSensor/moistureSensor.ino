
double soil_humidity;
int percent = 0;
int max_humidity = 700; //wet
int min_humidity = 1023; //dry

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
}

void loop() { 
soil_humidity = analogRead(A0);
Serial.println(soil_humidity);

percent = convertToPercent(soil_humidity);
Serial.print(percent);
Serial.println("%");

delay(1000);        // delay in between reads for stability
}

int convertToPercent(int value)
{
  int percentValue = 0;
  percentValue = map(value, min_humidity, max_humidity, 0, 100);
  return (percentValue/10);
}

// the loop routine runs over and over again forever:
/*void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1000);        // delay in between reads for stability
} */
