int incomingByte;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();
    if(incomingByte == 1)
    {
      Serial.print('H');
      delay(1000);
      Serial.print('L');
      delay(1000);
    }
   
  }
}
