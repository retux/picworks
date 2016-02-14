/* 
 * File:   soft_pwm_simple.c
 * Author: retux
 *
 * Created on 21 de enero de 2016, 20:57
 * Implements soft pwm in PIC 16F628A, irq source is TMR0 overflow.
 *
 *  Using PORTB as output RB0 to RB3.
 *  Duty cycle is defined through global variables (unsigned int pwm[0-X], with values
 * from 0 to 99. Further development may try 0-255 for using internet colors (to-do).
 *
 *
 */

#include <xc.h>
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


#define _XTAL_FREQ 4000000 // Freq XTAL 4Mhz
#define _DELAY 100

/* BOF Shadow register  */
union {
    uint8_t      Port;           /* acceso completo de sGPIO */
    struct {
        unsigned GP0 : 1;        /* acceso individual a cada pin */
        unsigned GP1 : 1;
        unsigned GP2 : 1;
        unsigned GP3 : 1;
        unsigned GP4 : 1;
        unsigned GP5 : 1;
        unsigned GP6 : 1;
        unsigned GP7 : 1;
    };
} sGPIO;
/* EOF shadow register*/

/* Duty cycle is defined here */
unsigned int pwm0 = 10;
unsigned int pwm1 = 25;
unsigned int pwm2 = 65;
unsigned int pwm3 = 99;
/* EOF duty cycle def */

unsigned int cont = 0;

void interrupt ISR(void)
{
    // tmr0 interrupt - pwm resolution is 100
    if (cont > 99)
        cont = 0;
    else
        cont++;
    // pwm0 output PORTBbits.RB0
    if ( cont > 0 && cont < pwm0 )
        sGPIO.GP0 = 1;
    else
        sGPIO.GP0 = 0;

    // pwm1 output PORTBbits.RB1
    if ( cont > 0 && cont < pwm1 )
        sGPIO.GP1 = 1;
    else
        sGPIO.GP1 = 0;

    // pwm2 output PORTBbits.RB2
    if ( cont > 0 && cont < pwm2 )
        sGPIO.GP2 = 1;
    else
        sGPIO.GP2 = 0;

// pwm3 output PORTBbits.RB3
    if ( cont > 0 && cont < pwm3 )
        sGPIO.GP3 = 1;
    else
        sGPIO.GP3 = 0;

    PORTB = sGPIO.Port;
    // unset interrupt flag
    INTCONbits.T0IF = 0;
    TMR0 = 254;
 }


int main(int argc, char** argv) {
    TRISB = 0; // PORTB is output
    PORTB = 0;
    //PORTBbits.RB0 = 1;
    //OPTION_REG = 0x0f; // Prescaler 1:256 asignado a TMR0
    INTCON = 0xA0;   // habilita interrupt TMR0
    OPTION_REGbits.T0CS = 0;
    INTCONbits.GIE = 1;     // enable Global interrupts
    INTCONbits.T0IE = 1; // enable TMR0 interrupt
    //OPTION_REGbits.PSA = 0;
    // prescaler assigned to TMR0
    //OPTION_REG |= 0x07;   // set the prescaler to 1:64 scale using an OR
    
    TMR0 = 254;
    while (1);

    return (EXIT_SUCCESS);
}

