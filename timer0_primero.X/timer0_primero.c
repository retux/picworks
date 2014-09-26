/* 
 * File:   timer0_primero.c
 * Author: matias
 *
 *  * Compilador: XC8 (con mplab)                  *
 * Este fuente usa el timer0 en funcion de timer propiamente dicho
 * Created on 30 de marzo de 2014, 18:20
 */
// Use project enums instead of #define for ON and OFF.
// CONFIG

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000 // Freq XTAL 4Mhz
#define _DELAY 500

/*
 * 
 */
unsigned int cnt, num;

void interrupt ISR(void)
{
    num++;            // Interrupt causa que num se incremente
    if ( num == 30 )
    {
        cnt++;      // 20 interrupciones incrementan cnt
        num = 0;
    }
    //INTCONbits.T0IF = 0;
    INTCONbits.T0IF = 0;
    
}
int main(int argc, char** argv)
{
    // Configuración del timer & interrupcion
   
   cnt = 0;
   num = 0;
   TRISB = 0;
   PORTB = 0;
   TMR0 = 0;
   OPTION_REG = 0x0f; // Prescaler 1:256 asignado a TMR0
   INTCON = 0xA0;   // habilita interrupt TMR0
   
   
   do
   {
       PORTB = cnt;
   } while ( PORTB < 0x0f); // mientras PORTB < 15

 return (EXIT_SUCCESS);
}

