 /*
  Arduino FS-I6X Demo
  fsi6x-arduino-uno.ino
  Read output ports from FS-IA6B receiver module
  Display values on Serial Monitor
  
  Channel functions by Ricardo Paiva - https://gist.github.com/werneckpaiva/
  
  DroneBot Workshop 2021
  https://dronebotworkshop.com
*/
 
// Define Input Connections
#define CH3 3 // Forward/Backward
#define CH4 6 // Left/Right

/*
Here is how I did that with the Flysky FS-I6X:

    Power on the transmitter, be sure to have all the controls in their default positions.
    Hold the OK key down for a few seconds until you display a Menu
    Use the Up and Down keys to select “Functions Setup”, the mode represented by a wrench.
    Select Functions Setup using the OK key
    Use the Up and Down controls to highlight Aux Channels
    Select Aux Channels with the OK key.
    You will be able to remap channels 5 and 6. Use the controls to select a channel, then cycle through the options for the controls you can attach to that channel.
    When you find one you want, just leave it. Then hold the Cancel key down for a few seconds and release it. You will exit the channels page and be back in the Functions menu.
    Choose Aux Channels again, just to be sure your setting was saved.
*/

// Integers to represent values from sticks and pots

int ch3Value;
int ch4Value;
 
// Boolean to represent switch value
bool ch6Value;
 
// Read the number of a specified channel and convert to the range provided.
// If the channel is off, return the default value
__attribute__((always_inline)) int readChannel(int channelInput, int minLimit, int maxLimit){
  return (map(pulseIn(channelInput, HIGH, 30000), 1000, 2000, minLimit, maxLimit));
}
 
// Read the switch channel and return a boolean value
__attribute__((always_inline)) bool readSwitch(byte channelInput){
  return (readChannel(channelInput, 0, 100) > 50);
}
 
void setup(){
  // Set up serial monitor
  Serial.begin(115200);
  
  // Set all pins as inputs
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
}

 
void loop() {
  
  // Get values for each channel
  ch1Value = readChannel(CH3, 100, -100);
  ch4Value = readChannel(CH4, 100, -100);

  
  // Print to Serial Monitor
  Serial.print("| Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" | Ch4: ");
  Serial.print(ch4Value);
  Serial.println(" |\n");
  
  delay(500);
}
