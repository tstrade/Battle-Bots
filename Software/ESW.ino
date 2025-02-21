//#include <Wire.h>
#include <Servo.h>
//#include <Arduino.h>
//#include "pitches.h"

/* >>> Motors & ESC >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

Servo Drive;
Servo Turn;
Servo Weapon;

#define S1_PIN        (   5)
#define S2_PIN        (   2)

#define FULL_REVERSE  (1000)
#define MEDH_REVERSE  (1175)
#define MEDL_REVERSE  (1325)

#define FULL_STOPPED  (1500)

#define MEDL_FORWARD  (1675)
#define MEDH_FORWARD  (1825)
#define FULL_FORWARD  (2000)

const short speeds[] = 
{
  FULL_REVERSE, MEDH_REVERSE, MEDL_REVERSE,
  FULL_STOPPED,
  MEDL_FORWARD, MEDH_FORWARD, FULL_FORWARD
};

void adjustMotors(void);

/* <<< Motors & ESC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> RC Receiver >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

#define FWD_CHANNEL   (   3) 
#define TRN_CHANNEL   (   6)
#define SWA_CHANNEL   (   7)
#define RCV_MIN       (1000)
#define RCV_MAX       (2000)

int BF_Value;
int LR_Value;
int SW_Value;

__attribute__((always_inline)) int readChannel(int channelInput);
__attribute__((always_inline)) bool readSwitch(void);
__attribute__((always_inline)) void monitorWeapon(void);

/* <<< RC Receiver <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Buzzer >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/*
#define BUZZER_PIN     ( 12)
#define EIGHTON        ( 75)
#define EIGHTOFF       ( 75)
#define QUARTON        (200)
#define QUARTOFF       ( 50)
#define HALFON         (400)
#define HALFOFF        (100)

const int gourmet_race_notes[] =
{
  // Measure 1
  NOTE_G6,  NOTE_F6, NOTE_DS6, NOTE_D6, NOTE_AS5, NOTE_G5,
  NOTE_C6,  NOTE_D6, NOTE_DS6, NOTE_F6, NOTE_D6,
  NOTE_C7,  NOTE_G6, 
  NOTE_DS6, NOTE_D6, NOTE_C6,
  NOTE_C6,  NOTE_D6, NOTE_DS6, NOTE_C6,
  // Measure 6
  NOTE_AS5, NOTE_C6, NOTE_G5,
  NOTE_C7,  NOTE_G6, 
  NOTE_DS6, NOTE_D6, NOTE_C6,  NOTE_C6, NOTE_D6,
  NOTE_DS6, NOTE_F6, NOTE_D6,  NOTE_C6, NOTE_AS5,
  NOTE_C6,  NOTE_G5, NOTE_C6,
  // Measure 11
  NOTE_C7,  NOTE_G6, 
  NOTE_DS6, NOTE_D6, NOTE_C6,
  NOTE_C6,  NOTE_D6, NOTE_DS6, NOTE_C6,
  NOTE_AS6, NOTE_C6, NOTE_G5,
  // Measure 16
  NOTE_C7,  NOTE_G6, 
  NOTE_DS6, NOTE_F6, NOTE_G6, NOTE_C6,
  NOTE_D6,  NOTE_F6, NOTE_D6, NOTE_AS5, NOTE_C6
};

const int gourmet_race_lengths[] =
{
  // Measure 1
  QUARTON, QUARTOFF, EIGHTON, EIGHTOFF, EIGHTON, EIGHTOFF, 
  EIGHTON, EIGHTOFF, EIGHTON, EIGHTOFF, QUARTON, QUARTOFF,
  // Measure 2
  EIGHTON, EIGHTOFF, EIGHTON, EIGHTOFF, EIGHTON, EIGHTOFF,
  EIGHTON, EIGHTOFF, HALFON,  HALFOFF,
  // Measure 3
  HALFON,  HALFOFF,  HALFON,  HALFOFF,
  // Measure 4
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, HALFON, HALFOFF,
  // Measure 5
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF,
  // Measure 6
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, HALFON, HALFOFF,
  // Measure 7
  HALFON,  HALFOFF,  HALFON,  HALFOFF,
  // Measure 8
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF,
  EIGHTON, EIGHTOFF, EIGHTON, EIGHTOFF,
  // Measure 9
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, EIGHTON, EIGHTOFF,
  EIGHTON, EIGHTOFF, QUARTON, QUARTOFF,
  // Measure 10
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, HALFON,  HALFOFF,
  // Measure 11
  HALFON,  HALFOFF,  
  // Measure 12
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, HALFON, HALFOFF,
  // Measure 13
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF,
  // Measure 14
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, HALFON,  HALFOFF,
  // Measure 15
  HALFON,  HALFOFF,  HALFON,  HALFOFF,
  // Measure 16
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF,
  // Measure 17
  QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF, QUARTON, QUARTOFF,
  // Measure 18
  HALFON,  HALFOFF
};
*/

void taunt(void);

/* <<< Buzzer <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Main Program >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

void setup()
{
  Serial.begin(9600);
  Drive.attach(S1_PIN);
  Turn.attach(S2_PIN);

  pinMode(FWD_CHANNEL, INPUT);
  pinMode(TRN_CHANNEL, INPUT);
  pinMode(SWA_CHANNEL, INPUT);
}

void loop()
{
  while(readSwitch());
  /*
  if (readSwitch()) {
    Drive.detach();
    Turn.detach();
  }
  */

  adjustMotors();
  delay(1000);
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

  Drive.writeMicroseconds(BF_Value);
  Turn.writeMicroseconds(LR_Value);
}

/* <<< Motor Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */



/* >>> Receiver Functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

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



/* >>> Buzzer Functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/*
void taunt()
{
  int i, j;
  for (i = 0; i < 61; i++) {
    j = i * 2;
    tone(BUZZER_PIN, gourmet_race_notes[i]);
    delay(gourmet_race_lengths[j]);
    noTone(BUZZER_PIN);
    delay(gourmet_race_lengths[j + 1]);
  }
}
*/
