/*
 * keypad.h
 *
 *  Created on: 26/03/2024
 *      Author: thinkpad
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick.h"
#include "swtimers.h"
#include "lcd.h"

#ifndef KEYPAD_H_
#define KEYPAD_H_

void keypad_init(void);
char scan_keypad(void);


#endif /* KEYPAD_H_ */
