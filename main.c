#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "lcd.h"
#include "keypad.h"


int charToInt(char c) {
    if(c >= '0' && c <= '9') {
        return c - '0';
    } else {
        return 0;
    }
}

int main(void) {
    init_systick(); // Initialize systick
    init_lcd();     // Initialize LCD
    keypad_init();  // Initialize keypad
    lcd_clear();    // Clear LCD

    lcd_print("Set Motor Speed");

    while(1) {
        char key = scan_keypad();  // Scan for key press

        if(key != '\0' && key != '*' && key != '#' ){
        	lcd_clear();
            char key_str[16] = "Motor Speed: "; // Make sure the array is large enough for the entire message
            int messageLength = strlen(key_str);
            key_str[messageLength] = key;
            key_str[messageLength + 1] = '\0';
            int motorspeed = charToInt(key); //  <----------- use this 'motorspeed' int for the actual motor
            lcd_print(key_str);
        }

    }
    return 0;
}
