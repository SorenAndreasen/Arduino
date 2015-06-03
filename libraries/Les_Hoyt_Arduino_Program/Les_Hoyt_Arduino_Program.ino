// Les-Høyt, software til knap
// written by Søren Andreasen, 2013, for BEK 
// sorenandreasen.com
#define BUTTON 3 // button (input) on pin 3 
#define LAMP 2 // lamp (output) on pin 2
// states: (0 = false/low, 1 = true/high) 
int switchState = 0;
int isRecording = 0;
int canStopRecording = 0;
int canStartRecording = 1;

void setup() {
pinMode(BUTTON, INPUT); // initialize our button pin as input 
pinMode(LAMP, OUTPUT); // initialize our lamp pin as output

Serial.begin(9600); // initialize serial comm
delay(1000); }
void loop() {
switchState = digitalRead(BUTTON); // read the state of our button (low/high)
// if buton is pushed and program isn't recording: Start recording
if (switchState == HIGH && isRecording == 0 && canStartRecording == 1) {
canStopRecording = 0; isRecording = 1;
}
// if button is pushed and program is recording: Stop recording
if (switchState == HIGH && isRecording == 1 && canStopRecording == 1) {
canStartRecording = 0;
isRecording = 0; 
}
//if recording: Turn on LED and send "1" out the serial port 
if (isRecording == 1)
{
digitalWrite(LAMP, HIGH);
Serial.print("1"); // this is the value we will listen for in Max 
Serial.println();
}
// if not recording: Turn off LED and send "0" out the serial port 
if (isRecording == 0)
{
digitalWrite(LAMP, LOW);
Serial.print("0"); // this is the value we will listen for in Max
Serial.println(); }
/// Release button (After Recording) 
if(switchState == 0 && isRecording == 1) {
canStopRecording = 1; }
/// Release button (After stop recording) 
if(switchState == 0 && isRecording == 0) {
canStartRecording = 1; }
delay(30); // debounce (increasing this might help if button becomes unstable) 
}
