/**
 ** tc_lib library
 ** Copyright (C) 2016
 **
 **   Antonio C. Domínguez Brito <adominguez@iusiani.ulpgc.es>
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
 * File: capture_test.ino 
 * Description: This is an example illustrating the use of the tc_lib library's
 * capture objects. Concretely captures the PWM output from an analog pin esta-
 * blished with an analog write.
 * Date: November 25th, 2015
 * Author: Antonio C. Dominguez-Brito <adominguez@iusiani.ulpgc.es>
 * ROC-SIANI - Universidad de Las Palmas de Gran Canaria - Spain
 */

#include "tc_lib.h"

using namespace arduino_due;

#define CAPTURE_TIME_WINDOW 2500000 // usecs
#define ANALOG_PIN 7
#define ANALOG_VALUE 127 // values in the interval [0,255] 

// capture_tc0 declaration
// IMPORTANT: Take into account that for TC0 (TC0 and channel 0) the TIOA0 is
// PB25, which is pin 2 for Arduino DUE, so  the capture pin in  this example
// is pin 2. For the correspondence between all TIOA inputs for the different 
// TC modules, you should consult uC Atmel ATSAM3X8E datasheet in section "36. 
// Timer Counter (TC)"), and the Arduino pin mapping for the DUE.
capture_tc0_declaration();

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // capture_tc0 initialization
  capture_tc0.config(CAPTURE_TIME_WINDOW);

  Serial.println("========================================================");

  Serial.print("ticks per usec: "); Serial.println(capture_tc0.ticks_per_usec());
  Serial.print("max capture window: "); 
  Serial.print(capture_tc0.max_capture_window());
  Serial.println(" usecs.");

  Serial.println("========================================================");

  analogWrite(ANALOG_PIN,ANALOG_VALUE);
}

void loop() {
  // put your main code here,to run repeatedly:

  uint32_t duty,period;
  capture_tc0.get_duty_and_period(duty,period);

  Serial.println("********************************************************");  
  Serial.print("--> duty: "); 
  Serial.print(duty/capture_tc0.ticks_per_usec());
  Serial.print(" usecs. period: ");
  Serial.print(period/capture_tc0.ticks_per_usec());
  Serial.println(" usecs.");
}

