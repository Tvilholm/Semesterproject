/*
 * lcd.c
 *
 *  Created on: 29/02/2024
 *      Author: thinkpad
 */

#include "lcd.h"


extern volatile INT16S ticks;

#define LCD_CLEAR        0x01 // Clear display
#define LCD_RETURN_HOME  0x02 // Return home command: Returns the cursor to the top left corner of the display
#define LCD_FUNCTION_SET 0x28 // Function set: 4-bit mode, 2 lines, 5x8 dots
#define LCD_ENTRY_MODE   0x06 // Entry mode: Increment cursor, no display shift
#define LCD_DISPLAY_ON   0x0C // Display on/off control command

// Defining the data pins connected to the LCD D4-D7
uint8_t LCD_D4_PIN = 0x10; // PC4 for LCD D4
uint8_t LCD_D5_PIN = 0x20; // PC5 for LCD D5
uint8_t LCD_D6_PIN = 0x40; // PC6 for LCD D6
uint8_t LCD_D7_PIN = 0x80; // PC7 for LCD D7

// Define the control pins
uint8_t LCD_RS_PIN = 0x04; // PD2 for LCD RS
uint8_t LCD_E_PIN  = 0x08; // PD3 for LCD E


// Function to create a blocking delay using a busy-wait loop
void delay_ms(INT16U milliseconds) {
    INT16S start_tick = ticks;

    while ((ticks - start_tick) < milliseconds) {
        // Just wait here until the time has passed
    }
}


void pulse_e_pin(){ // Pulsing LCD E pin, with micro delays
	delay_ms(0);        // Short delay
	GPIO_PORTD_DATA_R |= LCD_E_PIN;   // Set E high
	delay_ms(0);       // Short delay
	GPIO_PORTD_DATA_R &= ~LCD_E_PIN; // Set E low
	delay_ms(0);       // Short delay
}

void lcd_send_command(uint8_t command) {
    // Set RS to command mode
    GPIO_PORTD_DATA_R &= ~LCD_RS_PIN;

    // Send high nibble
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & 0x0F) | (command & 0xF0);
    pulse_e_pin();

    // Send low nibble
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & 0x0F) | ((command << 4) & 0xF0);
    pulse_e_pin();
}

void lcd_send_data(uint8_t data) {
    // Set RS to data mode
    GPIO_PORTD_DATA_R |= LCD_RS_PIN;

    // Send high nibble
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & 0x0F) | (data & 0xF0);
    pulse_e_pin();

    // Send low nibble
    GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & 0x0F) | ((data << 4) & 0xF0);
    pulse_e_pin();
}

void init_lcd() {
    // Initial hardware setup
    SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R2 | SYSCTL_RCGCGPIO_R3); // Clock setup
    volatile int delay = SYSCTL_RCGCGPIO_R;  // Delay for clock setup

    // Configure data direction and digital function for LCD data and control lines.
    GPIO_PORTC_DIR_R |= (LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN);
    GPIO_PORTC_DEN_R |= (LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN);
    GPIO_PORTD_DIR_R |= (LCD_RS_PIN | LCD_E_PIN);
    GPIO_PORTD_DEN_R |= (LCD_RS_PIN | LCD_E_PIN);

    // LCD initialization sequence
    delay_ms(40);  // Wait more than 40 ms after Vcc rises to 2.7V

    // Start in 8-bit mode to ensure clean start for LCD, need to send thrice.
    lcd_send_command(0x30);
    delay_ms(5);   // Wait more than 4.1 ms

    lcd_send_command(0x30);
    delay_ms(1);   // Wait more than 100 �s

    lcd_send_command(0x30);
    delay_ms(1);

    // Now switch to 4-bit mode
    lcd_send_command(0x20);
    delay_ms(1);

    // Continue in 4-bit mode, now send full function set command in two parts
    // High nibble (Function Set: DL = 0, N = 1, F = 0)
    lcd_send_command(LCD_FUNCTION_SET);
    delay_ms(1);

    // Low nibble (don't care bits, can be 0)
    lcd_send_command(0x00);
    delay_ms(1);

    // Display ON, Cursor OFF, Blink OFF
    lcd_send_command(LCD_DISPLAY_ON);
    delay_ms(1);

    // Clear display
    lcd_send_command(LCD_CLEAR);
    delay_ms(2);  // Clear command takes longer, around 1.52 ms, rounded up to 2 ms

    // Entry mode set: Increment cursor, no display shift
    lcd_send_command(LCD_ENTRY_MODE);
    delay_ms(1);
}

void lcd_clear() { // Clear the display
    lcd_send_command(LCD_CLEAR);  // Send the clear display command
    delay_ms(2);  // The clear command takes up to 1.52ms, delay is to ensure the command is processed
}

void set_cursor(int row, int col) { // Set the cursor position
    int address;
    switch (row) {
        case 0:
            address = 0x00 + col;  // 0x00 is the starting address of the first row
            break;
        case 1:
            address = 0x40 + col;  // 0x40 is the starting address of the second row
            break;
        default:
            address = 0x00;  // Default to first row if out of bounds
            break;
    }

    // Setting the DDRAM address
    lcd_send_command(0x80 | address);
    delay_ms(5);
}

void lcd_print(char *str) { // Print a string at the current cursor position
    while (*str) { // Continues until '\0'
        lcd_send_data((uint8_t)(*str));
        str++;  // Go to next index
        delay_ms(1); // Small delay to avoid buffer overflow.
    }
}

