/* 
 * File:   lcd4_bits.h
 * Author: retux
 * Fuente: http://www.microchip.com/forums/m685466.aspx
 *
 * Created on 9 de mayo de 2014, 20:22
 */

#ifndef LCD4_BITS_H
#define	LCD4_BITS_H

#ifdef	__cplusplus
extern "C" {
#endif



#ifdef	__cplusplus
}
#endif


#define DispRS    PORTBbits.RB0
#define DispE     PORTBbits.RB2
#define LCD_DAT_PORT PORTA

// Register selection
#define CMD_REG     0x00    // Command register
#define DATA_REG    0x01    // Data register
#define LCD_WRITE   0x00    // Writes to LCD
#define LCD_READ    0x01    // Reads from LCD


// LCD specific defines
#define LCD_CLR    0x01    // Clears display
#define LCD_HOME   0x02    // Returns home


// Entry mode set
#define LCD_DEC        0x04    // Decrements DDRAM address
#define LCD_SHIFT_R    0x05    // Shifts display to the right
#define LCD_INC        0x06    // Increments DDRAM address cursor from left to right?
#define LCD_SHIFT_L    0x07    // Shifts display to the left


// Display ON/OFF
#define LCD_DISPOFF    0x08    // Display off
#define LCD_DISPON    0x0C    // Display on
#define LCD_CRS_OFF    LCD_DISPON | 0x00    // Cursor does not display
#define LCD_CRS_ON    LCD_DISPON | 0x02    // Cursor displays
#define LCD_BL_OFF    LCD_DISPON | 0x00    // Cursor does not blink
#define LCD_BL_ON    LCD_CRS_ON | 0x01    // Cursor blinks



// Cursor and Display Shift
#define LCD_SH_C_L    0x10    // Shifts the cursor position to the left (Address Counter is decremented by 1)
#define LCD_SH_C_R    0x14    // Shifts the cursor position to the right (Address Counter is incremented by 1)
#define LCD_SH_D_L    0x18    // Shifts the entire display to the left. The cursor follows the display shift
#define LCD_SH_D_R    0x1C    // Shifts the entire display to the right. The cursor follows the display shift


// Function Set
#define LCD_FN_SET    0x28                // Function set. Data 4-bit
#define LCD_1L_5x8    LCD_FN_SET | 0x00    // Displays 1 line. Character font: 5x 8 dots
#define LCD_1L_5x10    LCD_FN_SET | 0x04    // Displays 1 line. Character font: 5x10 dots
#define LCD_2L_5x8    LCD_FN_SET | 0x08    // Displays 2 line. Character font: 5x 8 dots


// LCD defines for calling lcd_xy with line number, position and cursor status
#define LCD_LN1        0x80 //0x00    // DDRAM address of the 1st line
#define LCD_LN2        0xC0//0x40    // DDRAM address of the 2nd line


#define LCD_CG_SET    0x40    // Sets CG RAM address
#define LCD_DD_SET    0x80    // Sets DDRAM address
#define LCD_BUSY        0x80    // Busy flag


#define LCDNCHARS    16        // Define number of chars per line


// Function prototypes
void lcd_init(void);        // LCD initialization
void lcd_send_cmd(unsigned char data);    // Writes instruction to LCD
void lcd_send_dat(unsigned char data);    // Writes data to LCD


#endif	/* LCD4_BITS_H */

