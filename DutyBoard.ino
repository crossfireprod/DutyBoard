// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
 
// DEFINES
#define HEARTBEAT_LED 13
#define B_LED 2
#define N_LED 3
#define A_LED 5
#define Z_LED 7
#define L_LED 4
#define G_LED 6
#define ROPE_LED 8

#define DUTY_START 20 // Must be PM, expressed in 24 hour time.
#define DUTY_END 7    // Must be AM, expressed in 24 hour time.

// INCLUDES
#include <Wire.h>
#include "RTClib.h"
 
RTC_DS1307 RTC;

// GLOBAL VARIABLES
const char duty_calendar[] = { '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-' };
//                              0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31

void setup () {
  
  // I/O Init  
  pinMode(HEARTBEAT_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  pinMode(N_LED, OUTPUT);
  pinMode(A_LED, OUTPUT);
  pinMode(Z_LED, OUTPUT);
  pinMode(L_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(ROPE_LED, OUTPUT);
  
  // I2C Init
  Wire.begin();

  // RTC Init
  RTC.begin();

  //RTC.adjust(DateTime(__DATE__, (__TIME__)));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, (__TIME__)));
    //Serial.println("RTC SET");
  }

  // Serial Init
  Serial.begin(57600);
  Serial.println(" - Initialization Complete - ");

} // END Setup
 
void loop () {
  // Variable Declarations
  static char last_second = 0;
  
  // Get Current Time
  DateTime now = RTC.now();

  // Sleep
  delay(500);
    
  // Toggle Heartbeat LED, Print Current Time, and Update Outputs
  if (now.second() != last_second) {
    digitalWrite(HEARTBEAT_LED, !digitalRead(HEARTBEAT_LED));
    last_second = now.second();

    // Print Current Time
    PrintTime(now);

    // Is somebody on duty?
    if (IsDuty(now) == 1) {

      // Show that Duty == Yes
      digitalWrite(ROPE_LED, HIGH);      

      // Show who is on duty.
        // Ensure that all indicators are off.
        digitalWrite(B_LED, LOW);
        digitalWrite(N_LED, LOW);
        digitalWrite(A_LED, LOW);
        digitalWrite(Z_LED, LOW);
        digitalWrite(L_LED, LOW);
        digitalWrite(G_LED, LOW);

        // Illuminate Relevant Person Indicator
        switch (WhoDuty(now)) {
          case('B'):
            digitalWrite(B_LED, HIGH);
            Serial.println("DUTY: Bryan");
          break;

          case('N'):
            digitalWrite(N_LED, HIGH);
            Serial.println("DUTY: Natalie");
          break;

          case('A'):
            digitalWrite(A_LED, HIGH);
            Serial.println("DUTY: Austin");
          break;

          case('Z'):
            digitalWrite(Z_LED, HIGH);
            Serial.println("DUTY: Zach");
          break;

          case('L'):
            digitalWrite(L_LED, HIGH);
            Serial.println("DUTY: Liam");
          break;

          case('G'):
            digitalWrite(G_LED, HIGH);
            Serial.println("DUTY: Greg");
          break;

          default:
            // Leave all off.
          break;

        } // END who_duty switch

        Serial.println("");

    } else {
      // Show that Duty == No.
        digitalWrite(B_LED, LOW);
        digitalWrite(N_LED, LOW);
        digitalWrite(A_LED, LOW);
        digitalWrite(Z_LED, LOW);
        digitalWrite(L_LED, LOW);
        digitalWrite(G_LED, LOW);
        digitalWrite(ROPE_LED, LOW);
        Serial.println("DUTY: N/A");
        Serial.println("");

    }// END IsDuty if

  } // END Second Toggle if 

} // END Loop

char WhoDuty(DateTime now) {
  char who_duty;

  if (now.hour() >= DUTY_START){
    who_duty = duty_calendar[(int)now.day()];

  } else if (now.hour() < DUTY_END) {
    // Account for the early morning of a duty night taking place during the next calendar day.
    who_duty = duty_calendar[((int)now.day()) - 1];

  } else {
    who_duty = '-';

  }

  return who_duty;
} // END WhoDuty

char IsDuty(DateTime now) {
  char is_duty = 0;

  if ((now.hour() >= DUTY_START) || (now.hour() < DUTY_END)) {
    // Is duty, return true.
    is_duty = 1;

  } else {
    // Not Duty, return false.
    is_duty = 0;

  }

  return is_duty;
} // END IsDuty

void PrintTime(DateTime now) {
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC); 
  Serial.println();

} // END PrintTime
