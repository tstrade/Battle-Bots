#include <Servo.h>

/* >>> Motors & ESC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

Servo R_Wheel;
Servo L_Wheel;
Servo Weapon;

#define S1_PIN        (   5)
#define S2_PIN        (   2)

void adjustMotors(void);

/* <<< Motors & ESC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> RC Receiver >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

#define FWD_CHANNEL   (   6) 
#define TRN_CHANNEL   (   3)
#define SWA_CHANNEL   (   9)
#define RCV_MIN       (1000)
#define RCV_MAX       (2000)

#define FULL_REVERSE  (1000)
#define MEDH_REVERSE  (1150)
#define MEDM_REVERSE  (1300)
#define MEDL_REVERSE  (1400)

#define FULL_STOPPED  (1500)

#define MEDL_FORWARD  (1650)
#define MEDM_FORWARD  (1750)
#define MEDH_FORWARD  (1850)
#define FULL_FORWARD  (2000)

int BF_Value;
int LR_Value;
int SW_Value;

int dirFactor;

int mapToNearestSpeed(int speed);
__attribute__((always_inline)) int readChannel(int channelInput);
__attribute__((always_inline)) bool readSwitch(void);
__attribute__((always_inline)) void monitorWeapon(void);

/* <<< RC Receiver <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Main Program >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

void setup()
{
  Serial.begin(9600);
  R_Wheel.attach(S1_PIN);
  L_Wheel.attach(S2_PIN);

  R_Wheel.writeMicroseconds(FULL_STOPPED);
  L_Wheel.writeMicroseconds(FULL_STOPPED);

  pinMode(FWD_CHANNEL, INPUT);
  pinMode(TRN_CHANNEL, INPUT);
  pinMode(SWA_CHANNEL, INPUT);

  delay(500);
}

void loop()
{
  while(readSwitch()) {
    R_Wheel.writeMicroseconds(FULL_STOPPED);
    L_Wheel.writeMicroseconds(FULL_STOPPED);
    delay(500);
  }

  adjustMotors();
  delay(500);
}

/* <<< Main Program <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Motor Functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

void adjustMotors()
{ 
  int speed_left, speed_right;
  int speed_diff;

  LR_Value = mapToNearestSpeed(readChannel(TRN_CHANNEL));
  BF_Value = mapToNearestSpeed(readChannel(FWD_CHANNEL));
  speed_left = speed_right = BF_Value;

  /* -------------------------------------------------------------- */
  /*  ____________________________________________________________  */
  /* |                                                            | */
  /* | ! When BF_Value > FULL_STOPPED, bot is moving forwards   ! | */
  /* | ! When LR_Value > FULL_STOPPED, bot is turning right     ! | */
  /* |____________________________________________________________| */
  /*                                                                */
  /* -------------------------------------------------------------- */

  // Bot is turning left, i.e., right wheel moves faster than left
  if (LR_Value < FULL_STOPPED) {
    speed_right += dirFactor;
    speed_left -= dirFactor;

    if ((speed_diff = speed_right - FULL_FORWARD) > 0) {
      speed_right = FULL_FORWARD;
      speed_left -= speed_diff;
    }
    else if ((speed_diff = speed_right - FULL_REVERSE) < 0) {
      speed_right = FULL_REVERSE;
      speed_left += speed_diff;
    }
  }
  else if (LR_Value > FULL_STOPPED) {
    speed_left += dirFactor;
    speed_right -= dirFactor;

    if ((speed_diff = speed_left - FULL_FORWARD) > 0) {
      speed_left = FULL_FORWARD;
      speed_right -= speed_diff;
    }
    else if ((speed_diff = speed_left - FULL_REVERSE) < 0) {
      speed_left = FULL_REVERSE;
      speed_right += speed_diff;
    }
  }

  R_Wheel.writeMicroseconds(speed_right);
  L_Wheel.writeMicroseconds(speed_left);
}

/* <<< Motor Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Receiver Functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

int mapToNearestSpeed(int speed)
{
  if (speed < (FULL_STOPPED - 50)) {
    if (speed < (MEDH_REVERSE - 50)) {
      dirFactor = -60;
      return FULL_REVERSE;
    }
    else if (speed < (MEDM_REVERSE - 50)) {
      dirFactor = -55;
      return MEDH_REVERSE;
    }
    else if (speed < (MEDL_REVERSE - 50)) {
      dirFactor = -50;
      return MEDM_REVERSE;
    }
    else {
      dirFactor = -40;
      return MEDL_REVERSE;
    }
  }
  else if (speed < (MEDL_FORWARD - 50)) {
    dirFactor = 0;
    return FULL_STOPPED;
  }
  else {
    dirFactor = 40;
    if (speed < (MEDM_FORWARD - 50)) {
      return MEDL_FORWARD;
    }
    else if (speed < (MEDH_FORWARD - 50)) {
      dirFactor = 50;
      return MEDM_FORWARD;
    }
    else if (speed < (FULL_FORWARD - 50)) {
      dirFactor = 55;
      return MEDH_FORWARD;
    }
    else {
      dirFactor = 60;
      return FULL_FORWARD;
    }
  }
}

__attribute__((always_inline)) int readChannel(int channelInput) 
{
  return map(pulseIn(channelInput, HIGH, 30000), 980, 2020, RCV_MIN, RCV_MAX);
}

__attribute__((always_inline)) bool readSwitch()
{
  return (map(pulseIn(SWA_CHANNEL, HIGH, 30000), 1000, 2000, 0, 100) > 50);
} 

__attribute__((always_inline)) void monitorWeapon()
{
  return;
}

/* <<< Receiver Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


