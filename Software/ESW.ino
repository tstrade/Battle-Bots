#include <Wire.h>
#include <Servo.h>

// Servo motors (ESCs)
Servo Left, Right, Weapon;

#define minMotorSpeed 1000
#define maxMotorSpeed 2000
#define SHUTDOWN         0

// RC Controller
#define LEFT_RIGHT         6
#define FORWARD_BACKWARD  10
#define WEAPON_SWITCH     11

int left_right;
int fwd_bckwd;
bool weaponized;

__attribute__((always_inline)) int readChannel(int channelInput, int minLimit, int maxLimit);
__attribute__((always_inline)) int filterSpeed(int x_speed, int y_speed);
__attribute__((always_inline)) bool readSwitch(byte channelInput);
__attribute__((always_inline)) void monitorWeapon();

void setup()
{
  // Set up serial monitor
  Serial.begin(115200);
  Wire.begin();
  
  // Set all pins as inputs
  pinMode(LEFT_RIGHT, INPUT);
  pinMode(FORWARD_BACKWARD, INPUT);
  pinMode(WEAPON_SWITCH, INPUT);

  // Attach and arm ESCs
  Left.attach(3); 
  Right.attach(9);
  Weapon.attach(5);

  Left.writeMicroseconds(minMotorSpeed);
  Right.writeMicroseconds(minMotorSpeed);
  Weapon.writeMicroseconds(minMotorSpeed);

  Serial.println(F("Plug in the ESCs - wait for confirmation beeps"));
  //while (IRCode != 2) { checkIRCode(); }
  Serial.println(F("Servo motors successfuly armed -- System Ready!\n"));

  delay(1000);
}


void loop()
{
  adjustMotors();
  monitorWeapon();
  delay(500);
}

void adjustMotors() {
  int x_speed, y_speed;
  int speed_left, speed_right;
  
  x_speed =  readChannel(LEFT_RIGHT, minMotorSpeed, maxMotorSpeed);
  y_speed = readChannel(FORWARD_BACKWARD, minMotorSpeed, maxMotorSpeed);

  if (x_speed > 0) {
    speed_left = filterSpeed(x_speed, y_speed);
    speed_right = filterSpeed(x_speed, (-1 * y_speed));
  }
  else if (x_speed < 0) {
    speed_left = filterSpeed(x_speed, (-1 * y_speed));
    speed_right = filterSpeed(x_speed, y_speed);
  }
  else {
    speed_left = speed_right = 0;
  }

  Left.writeMicroseconds(speed_left);
  Right.writeMicroseconds(speed_right);
}

// Read the number of a specified channel and convert to the range provided.
// If the channel is off, return the default value
__attribute__((always_inline)) int readChannel(int channelInput, int minLimit, int maxLimit) 
{
  return map(pulseIn(channelInput, HIGH, 30000), -180, 180, minLimit, maxLimit);
}

__attribute__((always_inline)) int filterSpeed(int x_speed, int y_speed)
{
  return (((3 * x_speed) / 4) + (y_speed / 4));
}

// Read the switch channel and return a boolean value
__attribute__((always_inline)) bool readSwitch(byte channelInput)
{
  return (readChannel(channelInput, 0, 100) > 50);
} 

__attribute__((always_inline)) void monitorWeapon()
{
  return readSwitch(WEAPON_SWITCH) ? Weapon.writeMicroseconds(maxMotorSpeed) : Weapon.writeMicroseconds(SHUTDOWN);
}
