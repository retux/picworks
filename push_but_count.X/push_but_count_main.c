/* 
 * File:   push_but_count_main.c
 * Author: matias
 *
 * Created on 4 de abril de 2014, 20:36
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

#define _XTAL_FREQ 4000000 // Freq XTAL 4Mhz
#define _DELAY 50
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

unsigned char is_button1_pressed (void);
void setupOutput (void);


void setupPorts (void)
{
    // config puertos
    TRISA = 0xff;               // Todo el PORTA configurado como entradas
    TRISB = 0x00;
    PORTB = 0;
    CMCON = 0x07;   // Deshabilitamos los comparadores analógicos

    // Todo PORTB son salidas
    // EOF config puertos
    // Para que RA4 sea int no habrà que conf puerto dif?
}

unsigned char is_button1_pressed (void)
{   //Debouncing

    if ( PORTAbits.RA0 == 0 )
    {
        __delay_ms(_DELAY);

        if ( PORTAbits.RA0 == 0 )
        {
            return 1;   // sí: is_pressed!
        }
    }
    return 0;
}

void main(int argc, char** argv) {
    setupPorts();
    
    unsigned char cnt = 0;
    while (1)
    {        
        PORTB = sGPIO.Port;
        if ( is_button1_pressed() )
        {
              cnt++;
              if ( cnt > 15 )
                    cnt = 0;
              sGPIO.Port = cnt;
        }

     }
}

