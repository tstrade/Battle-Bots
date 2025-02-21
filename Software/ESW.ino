#include <Wire.h>
#include <Servo.h>
#include <pitches.h>

/* >>> Motors & ESC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

Servo L_Wheel;
Servo R_Wheel;
Servo Weapon;

#define S1_PIN        (   5)
#define S2_PIN        (   2)

void adjustMotors(void);

/* <<< Motors & ESC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> RC Receiver >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

#define FWD_CHANNEL   (   3) 
#define TRN_CHANNEL   (   6)
#define RCV_MIN       ( 100)
#define RCV_MAX       (-100)

int BF_Value;
int LR_Value;

__attribute__((always_inline)) int readChannel(int channelInput);
__attribute__((always_inline)) bool readSwitch(int channelInput);
__attribute__((always_inline)) void monitorWeapon(void);

/* <<< RC Receiver <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Buzzer >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

#define BUZZER_PIN     (  8)
#define EIGHTH_ON      ( 75)
#define EIGHTH_OFF     ( 75)
#define QUARTER_ON     (200)
#define QUARTER_OFF    ( 50)
#define HALR_ON        (400)
#define HALR_OFF       (100)


const int gourmet_race_notes[] =
{
  NOTE_G6,  NOTE_F6, NOTE_DS6, NOTE_D6, NOTE_AS5, NOTE_G5,
  NOTE_C6,  NOTE_D6, NOTE_DS6, NOTE_F6, NOTE_D6,
  NOTE_AS6, NOTE_G6, 
  NOTE_DS6, NOTE_D6, NOTE_C6,
  NOTE_C6,  NOTE_D6, NOTE_DS6, NOTE_C6,
  
  NOTE_AS5, NOTE_C6, NOTE_G5,
  NOTE_AS6, NOTE_G6, 
  NOTE_DS6, NOTE_D6, NOTE_C6,  NOTE_C6, NOTE_D6,
  NOTE_DS6, NOTE_F6, NOTE_D6,  NOTE_C6, NOTE_AS5,
  NOTE_C6,  NOTE_G5, NOTE_C6,
  
  NOTE_AS6, NOTE_G6, 
  NOTE_DS6, NOTE_D6, NOTE_C6,
  NOTE_C6,  NOTE_D6, NOTE_DS6, NOTE_C6,
  NOTE_AS6, NOTE_C6, NOTE_G5,
  
  NOTE_AS6, NOTE_G6, 
  NOTE_DS6, NOTE_F6, NOTE_G6, NOTE_C6,
  NOTE_D6,  NOTE_F6, NOTE_D6, NOTE_AS5, NOTE_C6
};

const int gourmet_race_


/* <<< Buzzer <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Main Program >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

void setup()
{
  L_Wheel.attach(S1_PIN);
  R_Wheel.attach(S2_PIN);

  pinMode(FWD_CHANNEL, INPUT);
  pinMode(TRN_CHANNEL, INPUT);
}

void loop()
{
  adjustMotors();
}

/* <<< Main Program <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Motor Functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

void adjustMotors()
{
  int speed, speed_left, speed_right;
  bool move_forwards, turn_right;
  
  move_forwards = ((BF_Value = readChannel(FWD_CHANNEL)) > 0);
  turn_right = ((LR_Value = readChannel(TRN_CHANNEL)) > 0);
  
  /* --------------------------------------------------- */
  /*  _________________________________________________  */
  /* |                                                 | */
  /* | ! When BF_Value > 0, bot is moving forwards   ! | */
  /* | ! When LR_Value > 0, bot is turning right     ! | */
  /* |_________________________________________________| */
  /* |                                                 | */
  /* | ! When LR > 0, speed_left > speed_right       ! | */
  /* | ! When LR < 0, speed_left < speed_right       ! | */
  /* |_________________________________________________| */
  /*                                                     */
  /* --------------------------------------------------- */

/*
  BF_Value *= 3;
  LR_Value *= 1;

  if (turn_right) {
    speed_left = (BF_Value + LR_Value) / 4;
    speed_right = (BF_Value - LR_Value) / 4;
  }
  else {
    speed_left = (BF_Value - LR_Value) / 4;
    speed_right = (BF_Value + LR_Value) / 4;
  }
*/

  L_Wheel.writeMicroseconds(BF_Value);
  R_Wheel.writeMicroseconds(LR_Value);
}

/* <<< Motor Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Receiver Functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

__attribute__((always_inline)) int readChannel(int channelInput) 
{
  //return map(pulseIn(channelInput, HIGH, 30000), 1000, 2000, RCV_MIN, RCV_MAX);
  return pulseIn(channelInput, HIGH, 30000);
}

__attribute__((always_inline)) bool readSwitch(int channelInput)
{
  return (map(pulseIn(channelInput, HIGH, 30000), 1000, 2000, 0, 100) > 50);
} 

__attribute__((always_inline)) void monitorWeapon()
{
  return;
}

/* <<< Receiver Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
