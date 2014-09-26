/* 
 * File:   xlcd_dyn_display_main.c
 * Author: matias
 *
 * Created on 10 de septiembre de 2014, 20:59
 */


//CON ESTE ORDEN DE CONFIGURACION FUNCIONA RB5..... Para LCD
#pragma config PLLDIV = 1, CPUDIV = OSC1_PLL2, USBDIV = 1
#pragma config FOSC = HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = OFF, BOR = OFF, VREGEN = OFF
#pragma config WDT = OFF, WDTPS = 32768
#pragma config MCLRE = ON, LPT1OSC = OFF, PBADEN = OFF
#pragma config STVREN = ON, LVP = OFF, XINST = OFF
// Atenti. Tuve que usar estos config bits, porque con la config que me daba la herramienta de mplab
// había algun problema, quizá con RB5, como dice este blog:
// http://unasguiasmas.wordpress.com/2014/03/20/01-libreria-xlcd-para-el-manejo-de-displays-lcd/

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <plib/xlcd.h>
#include <plib/delays.h>

#define _XTAL_FREQ 20000000

//Retardos requeridos por la librería XLCD
void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);
// funcion propia para delay
void Delay_ms(unsigned int);


void main(int argc, char** argv) {
    // sprintf puede usarse para convertir el valor devuelto por un sensor.
    // Test
    //float n = 20.12;
    //char buffer[5];
    //sprintf(buffer, "%2.f", n);

    // eof test

    /* BOF: configuracion de puertos. La lib xlcd usa el portB */
    /* de lo contrario, hay que alterar el xlcd.h creo         */
    // El config PBADEN en ON el PORTB se configura en digital (entradas AN deshabilitadas)
    // Por si acaso, todo configurado en digital ADCON1.PCFG<1:3>

    ADCON1bits.PCFG0 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 1;
    ADCON0bits.ADON = 0;
    ADCON1 = 0b00001111;
    //
    // Por si acaso deshabilitamod los comparadores
    CMCONbits.CM2 = 1;
    CMCONbits.CM1 = 1;
    CMCONbits.CM0 = 1;
    TRISB = 0;
    //TRISBbits.RB0 = 0;  // Pin RB0 para salida
    //LATBbits.LATB0 = 0;
    /* EOF: Fin config*/

    unsigned int cont = 0;
    char buffer[6];         // Max = 99999, recordar el final de string \0, por eso sizeof(buffer)=6;
    unsigned int max = 99999;

    Delay_ms(1000);
    //Configurando LCD 4 bits mutilínea
    OpenXLCD(FOUR_BIT & LINES_5X7);
    
    //Esperar hasta que el display esté disponible.
    while(BusyXLCD());
    //Mover cursor a la derecha...
     WriteCmdXLCD(0x06);
    //Desactivando el cursor.
    WriteCmdXLCD(0x0C);
    __delay_ms(10);
    //Segunda línea
    //SetDDRamAddr(0x40);
    //putrsXLCD("Contando...");

    while (1)
    {
        cont++;
        //Primera línea
        SetDDRamAddr(0x05);
        sprintf(buffer, "%05d", cont);
        putrsXLCD(buffer);
        Delay_ms(1000);
        if ( cont==65535 ) cont=0; 
        //putrsXLCD("Que pasha retux?");
    }

    
}

void DelayFor18TCY(void)
{
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}

void DelayPORXLCD(void)
{
    // Delay of 15ms
    // Cycles = (TimeDelay * Fosc) / 4
    // Cycles = (15ms * 20MHz) / 4
    // Cycles = 75000   Pero como el display es lenteja le doy 50% mas
    Delay1KTCYx(110);
    //Delay1KTCYx(125);
    return;         // este return se puede sacar (?) es void.
}

void DelayXLCD(void)
{
    // Delay of 5ms
    // Cycles = (TimeDelay * Fosc) / 4
    // Cycles = (5ms * 20MHz) / 4
    // Cycles = 25,000
     //Delay1KTCYx(25);
     Delay1KTCYx(75); // Probe darle mas changui
     return;
}

void Delay_ms(unsigned int ms)
{
    for (unsigned int i=0; i<ms; i++)
        __delay_ms(1);
}

