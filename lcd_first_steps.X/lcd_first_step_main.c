/* 
 * File:   lcd_first_step_main.c
 * Author: retux
 *
 * Fuente: http://www.microchip.com/forums/m685466.aspx
 * Created on 9 de mayo de 2014, 20:20
 * Esta version funciona OK. Pero tener en cuenta que funciona basandose en el timing
 * Una lib que podria hacerse podria leer el busy flag y tener algunas otras funciones.
 */
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "lcd4_bits.h"

#define _XTAL_FREQ 4000000 // Freq XTAL 4Mhz

/*
 * 
 */

void lcd_init()
{
    __delay_ms(120);
    lcd_send_cmd(0x03);
    __delay_ms(15);
    lcd_send_cmd(0x03);
    __delay_ms(5);
    lcd_send_cmd(0x03);
    __delay_us(50);
    lcd_send_cmd(0x02);
    lcd_send_cmd(LCD_FN_SET);
    lcd_send_cmd(LCD_DISPOFF); //0x08
    lcd_send_cmd(LCD_CLR); //0x01
    lcd_send_cmd(LCD_DISPON); //0x0C
    // BOF test de entry mode para scroll
    //lcd_send_cmd(LCD_INC);
    lcd_send_cmd(LCD_SHIFT_L);
    
    
    // EOF test de entry mode
}

void pulseEnable()
{
    DispE = 0;
    __delay_us(3);
    DispE = 1;
    __delay_us(3);
    DispE = 0;
    __delay_us(200);
}

void lcd_send_dat(uint8_t value)
{
    DispE = 0;
    DispRS = 1; //1 per mandare dati
    unsigned char value2 = (value>>4);
    PORTAbits.RA0 = (value2 >> 0) & 0x01;
    PORTAbits.RA1 = (value2 >> 1) & 0x01;
    PORTAbits.RA2 = (value2 >> 2) & 0x01;
    PORTAbits.RA3 = (value2 >> 3) & 0x01;
    pulseEnable();

    PORTAbits.RA0 = (value >> 0) & 0x01;
    PORTAbits.RA1 = (value >> 1) & 0x01;
    PORTAbits.RA2 = (value >> 2) & 0x01;
    PORTAbits.RA3 = (value >> 3) & 0x01;
    pulseEnable();
}

void lcd_send_cmd(uint8_t value)
{
    DispE = 0;
    DispRS = 0; //0 per mandare comandi
    unsigned char value2 = (value>>4);

    PORTAbits.RA0 = (value2 >> 0) & 0x01;
    PORTAbits.RA1 = (value2 >> 1) & 0x01;
    PORTAbits.RA2 = (value2 >> 2) & 0x01;
    PORTAbits.RA3 = (value2 >> 3) & 0x01;
    pulseEnable();

    PORTAbits.RA0 = (value >> 0) & 0x01;
    PORTAbits.RA1 = (value >> 1) & 0x01;
    PORTAbits.RA2 = (value >> 2) & 0x01;
    PORTAbits.RA3 = (value >> 3) & 0x01;

    pulseEnable();
    __delay_ms(10);
}

void writeString(unsigned char *msg)
{

    while(*msg!='\0')
    {
        lcd_send_dat(*msg++);
    }
}

int main(int argc, char** argv) {

 TRISA = 0x00;
 PORTA = 0x00;

 TRISB = 0x00;
 PORTB = 0x00;
 INTCON = 0x00;
 CMCON = 0b00000111; //disabilito i comparatori
 PIE1 = 0x00;

 lcd_init();

while(1)
{
    lcd_send_cmd(LCD_HOME);
    lcd_send_cmd(LCD_CLR);
    writeString("Hola retux :)");
    //lcd_send_cmd(0b00010000);

    
    lcd_send_cmd(LCD_LN2);
    writeString("Que te pasha...?     ");


    for (char i=0; i<10; i++)
    {
        lcd_send_cmd(LCD_SH_D_L);
        __delay_ms(300);
    }

    __delay_ms(3000);
    //writeString("Estas nerviosho?");
    

}

return (EXIT_SUCCESS);
}
