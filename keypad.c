/*
 * lcd.c
 *
 *  Created on: 29/02/2024
 *      Author: thinkpad
 */

#include "keypad.h"

#define ROWS (GPIO_PORTE_DATA_R)
#define COLUMNS (GPIO_PORTA_DATA_R)

void keypad_init(void) {

	lcd_print("#");
	delay_ms(50);
    SYSCTL_RCGCGPIO_R |= (1U << 0) | (1U << 4); // Enable clock for Port E and A
    lcd_print("#");
    delay_ms(50);
    GPIO_PORTA_DIR_R &= ~0x1C; 	// Initialize column pins (PA2-PA4) as inputs with pull-up resistors
    lcd_print("#");
    delay_ms(50);
    GPIO_PORTA_DEN_R |= 0x1C;    // Enable digital function for PA2-PA4
    lcd_print("#");
    delay_ms(50);
    GPIO_PORTA_PUR_R |= 0x1C;    // Enable pull-up for PA2-PA4
    lcd_print("#");
    delay_ms(50);
    GPIO_PORTE_DIR_R |= 0x0F; 	// Initialize row pins (PE0-PE3) as outputs
    lcd_print("#");
    delay_ms(50);
    GPIO_PORTE_DEN_R |= 0x0F;    // Enable digital function for PE0-PE3
    lcd_print("#");
    delay_ms(50);
    GPIO_PORTE_DATA_R |= 0x0F; 	// Ensure rows are set high (inactive) initially
    lcd_print("#");
    delay_ms(50);
}

char scan_keypad(void) {
	const char keys[4][3] = {
	    {'#', '0', '*'},
	    {'9', '8', '7'},
	    {'6', '5', '4'},
	    {'3', '2', '1'}
	};

    int row;
    for (row = 0; row < 4; row++) {
        // Drive one row (PE0-PE3) low
        GPIO_PORTE_DATA_R = ~(1U << row);
        delay_ms(2); // Debounce delay
        int col;
        for (col = 0; col < 3; col++) {
            if (!(GPIO_PORTA_DATA_R & (1 << (col + 2)))) { // If button is pressed
                GPIO_PORTE_DATA_R = 0x0F; // Reset rows to high before exit
                return keys[row][col];
            }
        }
        // Reset row to high before moving to next
        GPIO_PORTE_DATA_R = 0xFF;
    }
    return '\0'; // No key pressed
}
