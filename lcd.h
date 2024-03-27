/*
 * lcd.h
 *
 *  Created on: 29/02/2024
 *      Author: thinkpad
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick.h"
#include "swtimers.h"

#ifndef LCD_H_
#define LCD_H_

void delay_ms(INT16U milliseconds); // Delay function.
void pulse_e_pin(); // Pulsing LCD E pin, with micro delays.
void lcd_send_command(uint8_t command); // Sending command to LCD with RS = 0
void lcd_send_data(uint8_t data); // Sending data to LCD with RS = 1
void lcd_init(void);  // Initialize the LCD
void lcd_clear(void); // Clear the display
void lcd_set_cursor(int row, int col);  // Set the cursor position
void lcd_print(char *str);  // Print a string at the current cursor position


#endif /* LCD_H_ */
