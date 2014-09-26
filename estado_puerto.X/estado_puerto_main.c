/* 
 * File:   estado_puerto_main.c
 * Author: matias
 *
 * Created on 5 de abril de 2014, 17:03
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
/*
 * 
 */
int main(int argc, char** argv) {
    TRISB = 0x00;
    PORTB = 0;
    sGPIO.Port = 0b00001010;
    while(1)
    {
        PORTB = sGPIO.Port;
    }
    return (EXIT_SUCCESS);
}

