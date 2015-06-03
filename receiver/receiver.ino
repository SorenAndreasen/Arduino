int ledPin = 13;
int incomingByte;

void setup()
{
 pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    if(incomingByte == 'H') digitalWrite(ledPin, HIGH);
    else if(incomingByte == 'I') digitalWrite(ledPin, LOW);
  }
  
  
}
