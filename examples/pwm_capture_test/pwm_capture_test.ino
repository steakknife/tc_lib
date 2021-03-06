/**
 ** tc_lib library
 ** Copyright (C) 2015,2020
 **
 **   Antonio C. Domínguez Brito <antonio.dominguez@ulpgc.es>
 **     División de Robótica y Oceanografía Computacional <www.roc.siani.es>
 **     and Departamento de Informática y Sistemas <www.dis.ulpgc.es>
 **     Universidad de Las Palmas de Gran  Canaria (ULPGC) <www.ulpgc.es>
 **  
 ** This file is part of the tc_lib library.
 ** The tc_lib library is free software: you can redistribute it and/or modify
 ** it under  the  terms of  the GNU  General  Public  License  as  published  by
 ** the  Free Software Foundation, either  version  3  of  the  License,  or  any
 ** later version.
 ** 
 ** The  tc_lib library is distributed in the hope that  it  will  be  useful,
 ** but   WITHOUT   ANY WARRANTY;   without   even   the  implied   warranty   of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE.  See  the  GNU  General
 ** Public License for more details.
 ** 
 ** You should have received a copy  (COPYING file) of  the  GNU  General  Public
 ** License along with the tc_lib library.
 ** If not, see: <http://www.gnu.org/licenses/>.
 **/
/*
 * File: pwm_capture_test.ino 
 * Description: This is an example to test how fast are the digital 
 * signals tc_lib is able to measure. For doing that it uses library
 * pwm_lib at https://github.com/antodom/pwm_lib.
 * Date: February 1st, 2015
 * Author: Antonio C. Dominguez-Brito <antonio.dominguez@ulpgc.es>
 * ROC-SIANI - Universidad de Las Palmas de Gran Canaria - Spain
 */

#include "tc_lib.h"
#include "pwm_lib.h"

using namespace arduino_due::pwm_lib;

#define PWM_PERIOD_PIN_35 500 // hundredths of usecs (1e-8 secs)
#define PWM_DUTY_PIN_35 50 // hundredths of usecs (1e-8 secs)

#define DUTY_KEEPING_TIME 1000 // msecs

// defining pwm object using pin 35, pin PC3 mapped to pin 35 on the DUE
// this object uses PWM channel 0
pwm<pwm_pin::PWMH0_PC3> pwm_pin35;

// To measure PWM signals generated by the previous pwm objects, we will use
// a capture object of tc_lib library as "oscilloscope" probe:  capture_tc0. 
// IMPORTANT: Take into account that for TC0 (TC0 and channel 0) the TIOA0 is
// PB25, which is pin 2 for Arduino DUE, so capture_tc0's capture pin is pin
// 2. For the correspondence between all TIOA inputs for the different
// TC module channels, you should consult uC Atmel ATSAM3X8E datasheet
// in section "36. Timer Counter (TC)"), and the Arduino pin mapping 
// for the DUE.
// All in all, to meausure the pwm output in this example you should connect 
// the PWM output of pin 35 to capture_tc0 object pin 2.
capture_tc0_declaration(); // TC0 and channel 0
auto& capture_pin2=capture_tc0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // initialization of capture objects
  capture_pin2.config((PWM_PERIOD_PIN_35/100)<<1);

  // starting PWM signals
  pwm_pin35.start(PWM_PERIOD_PIN_35,PWM_DUTY_PIN_35);
}

// FIX: function template change_duty is defined in
// #define to avoid it to be considered a function
// prototype when integrating all .ino files in one
// whole .cpp file. Without this trick the compiler
// complains about the definition of the template
// function.
#define change_duty_definition \
  template<typename pwm_type> void change_duty( \
        pwm_type& pwm_obj, \
        uint32_t pwm_duty, \
        uint32_t pwm_period \
      ) \
{ \
    uint32_t duty=pwm_obj.get_duty()+pwm_duty; \
    if(duty>pwm_period) duty=pwm_duty; \
    pwm_obj.set_duty(duty); \
}
// FIX: here we instantiate the template definition
// of change_duty
change_duty_definition;

void loop() {
  // put your main code here,to run repeatedly:

  delay(DUTY_KEEPING_TIME);

  uint32_t status,duty,period;

  Serial.println("=======================================================================");
  Serial.print("[PIN 35 -> PIN 2] "); 
  status=capture_pin2.get_duty_and_period(duty,period);
  Serial.print("duty: "); 
  Serial.print(
    static_cast<double>(duty)/
    static_cast<double>(capture_pin2.ticks_per_usec()),
    3
  );
  Serial.print(" usecs. period: ");
  Serial.print(
    static_cast<double>(period)/
    static_cast<double>(capture_pin2.ticks_per_usec()),
    3
  );
  Serial.print(" usecs. ");
  if(capture_pin2.is_overrun(status)) Serial.print("[overrun]");
  if(capture_pin2.is_stopped(status)) Serial.print("[stopped]");  
  Serial.println();   
  Serial.println("=======================================================================");

  // changing duty in pwm output pin 35 
  change_duty(pwm_pin35,PWM_DUTY_PIN_35,PWM_PERIOD_PIN_35);
}

