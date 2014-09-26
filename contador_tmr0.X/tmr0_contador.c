/* 
 * File:   tmr0_contador.c
 * Author: retux
 *
 * Created on 29 de marzo de 2014, 22:43
 * Compilador: XC8 (con mplab)                  *
 */

// #pragma config statements should precede project file includes.
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
#include <stdint.h>


#define _XTAL_FREQ 4000000 // Freq XTAL 4Mhz
#define _DELAY 100
/*
 * 
 */

/* copia sombra de GPIO */
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


int main(int argc, char** argv) {
    INTCONbits.GIE = 1;         // Habilita irq Globales
    INTCONbits.PEIE = 1;      // habilita todas las unmasked IRQ de perif.
    OPTION_REGbits.T0CS = 1;    // RA4 sera la fuente para incremento de TMR0
    OPTION_REGbits.T0SE = 1;    // Seleccionamos falling edge para incremento

    PORTA = 0;
    //TRISA = 0xff;               // Todo el PORTA configurado como entradas
    TRISA = 0b00010100;           // RA4 sera input (T0CK) y RA2 Reset del timer
    PORTB = 0;
    TRISB = 0x00;                  // Todo PORTB son salidas
    CMCON = 0x07;   // Deshabilitamos los comparadores analógicos
    
    do
    {
        if ( TMR0 > 15 )
            TMR0 = 0;
       if (PORTAbits.RA2 == 0)
        {
            __delay_ms(50);
            if (PORTAbits.RA2 == 0)
                TMR0 = 0;
        }
    
        sGPIO.Port = TMR0;
        PORTB = sGPIO.Port;
        
    } while (1);
return (EXIT_SUCCESS);
}

