/* 
 * File:   sensor_temp_adc_comm_main.c
 * Author: retux
 * MCU: 18F2550
 * Esta version agrega comunicacion uart (9600 N81) con int. para que el mcu
 * solo interrumpa su loop en caso de que haya recibido un pedido por el puerto
 * serie. En lo demas es parecido al prog. sensor_temp01_main.c
 * Created on 24 de septiembre de 2014, 20:31
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
#include <plib/usart.h>

#define _XTAL_FREQ 20000000

//Retardos requeridos por la librería XLCD
void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);
// funcion propia para delay
void Delay_ms(unsigned int);
void setupMCU(void);

// Globales
char* pBuffer;

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
     /* BOF: config de uart (puerto serie)*/
    TRISCbits.RC6 = 0;  // RC6 / TX es Output
    TRISCbits.RC7 = 1;  // RC7 / RX es Input
    RCSTAbits.SPEN = 1;
    // Para interrupciones, ademas ver los primeros parametros de OpenUSART
    RCIF = 0;   // reset RX flag
    RCIP = 0;   // No alta prioridad
    RCIE = 1;   // habilitamos RX irq
    PEIE = 1;   // Habilitamos irq para perifericos
    ei();   // Master interrupt switch
    /* EOF: config de uart (puerto serie)*/
}

void main (void) {
    setupMCU();

    float temp = 0;
    unsigned int result = 0;
    char buffer[6] = "";    // Max = 99999, recordar el final de string \0, por eso sizeof(buffer)=6;
    pBuffer = buffer;

    Delay_ms(1000);     // Esperamos un toque, para que el lcd tenga tiempo de iniciarse.
    /*BOF: inicialización de puerto uart, 9600 N81 */
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH, 129);
    Delay10TCYx(5);
    putsUSART("Ready \n\r");
    /* EOF: inicialización de puerto uart el valor SPRG = 129 sale de la tabla 20-3 (pag. 243) del datasheet
     * del 18F2550. Vease que habilitamos irq de RX. Baudrate: 9600 N81. */
    
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
        sprintf(pBuffer, "%2.1f", temp);
        putrsXLCD(pBuffer);
        //__delay_us(200);
        //putsUSART(pBuffer);
        //putsUSART("\r");
        Delay_ms(300);  // Como estamos haciendo polling vamos a darle un tiempo de refresco al lcd.

    }
}

void interrupt SerialRxPinInterrupt()
{
    char rx;
    if ( PIR1bits.RCIF == 1)
    {
        rx = ReadUSART();

        __delay_us(200);
        putsUSART(pBuffer);
        putsUSART("\n\r");
        //WriteUSART(rx);
        //putcUSART('\r');
        //putsUSART(pBuffer);
        PIR1bits.RCIF = 0;  // borramos el flag rx
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


