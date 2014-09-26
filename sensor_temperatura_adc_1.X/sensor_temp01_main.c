/* 
 * File:   sensor_temp01_main.c
 * Author: retux
 * Descripción: lee la temperatura a partir de un sensor LM35, usando ADC AN0 (portA)
 * e imprimiendo el resultado en display LCD.
 * LM35 entrega 10mV/grado C. Ej. a 25C->250mV.
 * NOTA sobre la conversión. Si el ADC tiene muestreo de 10 bits, entonces tendremos 2^10 = 1024
 * escalones o niveles discretos. 5V/1024 = 0.0048V, cada nivel será de 0.0048V.
 * Los límites sup e inferior de esa escala son 5 y 0v respectivamente, que son los vref, que
 * se configuraron para ser Vdd y Vss.
 * Si lectura = ReadADC(); // lectura de la conversión
 * Entonces el valor de tensión: voltios=5.0*lectura/1024.0;      //
 * y la grados C=voltios*100;
 * Created on 13 de septiembre de 2014, 17:12
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
#include <plib/adc.h>
#include <plib/delays.h>

#define _XTAL_FREQ 20000000

//Retardos requeridos por la librería XLCD
void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);
// funcion propia para delay
void Delay_ms(unsigned int);
void setupMCU(void);

void setupMCU(void) {

    // Por si acaso deshabilitamod los comparadores
    CMCONbits.CM2 = 1;
    CMCONbits.CM1 = 1;
    CMCONbits.CM0 = 1;
    TRISB = 0;  // PortB Output
    TRISAbits.RA0 = 1;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 1;

    // Config basica de ADC, el resto lo va a hacer la lib
    ADCON0 = 0b00000001;    // seleccionamos AN0 y habilita ADC
    ADCON0bits.ADON = 1;    // es redundte respecto de la instrucciòn anterior

    ADCON1 = 0b00001100;    // AN2:AN0 habilitados. Vref a Vdd y vss
    // restaria configurar ADCON2, para clock de adc, y TAD, pero lo va a hacer la lib
}

void main (void) {
    setupMCU();

    unsigned int result = 0;
    float temp = 0;
    char buffer[6];         // Max = 99999, recordar el final de string \0, por eso sizeof(buffer)=6;
    unsigned int max = 99999;

    Delay_ms(1000);
    // BOF config de lib adc.h
    //Configure ADC

    /*Configuración del módulo AD
     * Fosc = 64
    * Alineación = derecha
    * 16 TAD
     * Canal AN0
    * Interrupción deshabilitada
    * VREF+ y VREF- conectados a VDD y VSS respectivamente
     * Valor de ADCON1 = 14 (Canal AN0 analógico, el resto digitales)
    */
    OpenADC(ADC_FOSC_64 &
     ADC_RIGHT_JUST &
     ADC_16_TAD,
     ADC_CH0 &
     ADC_INT_OFF &
     ADC_VREFPLUS_VDD &
     ADC_VREFMINUS_VSS,
    14);        // con ADCON1 = 14 (1110), los pines del portA quedan digitales excepto AN0
    //Retardo de 50 Tcy
    Delay10TCYx(5);
     
    // EOF config lib, en este caso no usamos irq, sino polling


    //Configurando LCD 4 bits mutilínea
    OpenXLCD(FOUR_BIT & LINES_5X7);

    //Esperar hasta que el display esté disponible.
    while(BusyXLCD());
    //Mover cursor a la derecha...
     WriteCmdXLCD(0x06);
    //Desactivando el cursor.
    WriteCmdXLCD(0x0C);
    __delay_ms(10);
    
    //SetDDRamAddr(0x40);
    SetDDRamAddr(0x00);
    __delay_us(200);
    putrsXLCD("Temp: ");
    __delay_us(200);
    SetDDRamAddr(0x0D);
    putrsXLCD("C");
    
    while (1)
    {
        //Iniciar la conversión
        ConvertADC();
         //Espera para que se complete la conversión
        while(BusyADC());
 
        //Captura del resultado   - Ver comentario al principio
        result = ReadADC();
        temp = (5.0 * result / 1024.0) * 100;

        SetDDRamAddr(0x07);
        sprintf(buffer, "%2.1f", temp);
        putrsXLCD(buffer);
        Delay_ms(300);  // Como estamos haciendo polling vamos a darle un tiempo de refresco al lcd.
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


