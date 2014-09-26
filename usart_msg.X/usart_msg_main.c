/* 
 * File:   usart_msg_main.c
 * Author: matias
 *
 * Created on 19 de abril de 2014, 22:28
 */
#pragma config FOSC = XT      // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF     // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON     // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON     // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF    // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF      // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled)
#pragma config CPD = OFF      // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF       // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _XTAL_FREQ 4000000 // Freq XTAL 4Mhz

/*
 * 
 */

char caracter;
bit flag_interrupcao = 0;
///////////////////////////////////////////////////interrupção//////////////////////////////////////////////////////////////
void interrupt RS232(void)//vetor de interrupção
 {
     caracter = RCREG;
     flag_interrupcao = 1;
     RCIF = 0;//  limpa flag de interrupção de recepção
 }

/////////////////////////////////funçoes usadas pela uart //////////////////////////////////////////////////////
void inicializa_RS232(long velocidade,int modo)
{   /*  Por padrão é usado o modo 8 bits e sem paridade, mas se necessario ajuste
     *  aqui a configuração desejada.
     *  verifique datasheet para ver a porcentagem de erro e se a velocidade é
     *  possivel para o cristal utilizado.
    */
    RCSTA = 0x90;  //habilita porta serial (RCSTA.SPEN=1),recepção de 8 bit em
                   // modo continuo (RCSTA.CREN = 1), asincronico.

    int valor;
        if(modo == 1)
        {//modo = 1 ,modo alta velocidade
         TXSTA = 0x24;//modo assincrono,trasmissao 8 bits.
         valor =(int)(((_XTAL_FREQ/velocidade)-16)/16);//calculo do valor do gerador de baud rate
        }
        else
        {//modo = 0 ,modo baixa velocidade
         TXSTA = 0x20;//modo assincrono,trasmissao 8 bits.
         valor =(int)(((_XTAL_FREQ/velocidade)-64)/64);//calculo do valor do gerador de baud rate
        }
    SPBRG = valor;
    RCIE = 1; //habilita interrupção de recepção
    TXIE = 0; //deixa interrupção de transmissão desligado(pois corre se o risco de ter
              //uma interrupção escrita e leitura ao mesmo tempo)
}
void escreve(char valor)
{
    TXIF = 0;//limpa flag que sinaliza envio completo.
    TXREG = valor;
    while(TXIF ==0);//espera enviar caracter
}
void imprime(const char frase[])
{
     char indice = 0;
     char tamanho = strlen(frase);
      while(indice < tamanho )
       {
           escreve(frase[indice]);
           indice++;
       }
}

void main(void)
{
    TRISB = 0X02;//configura portB  B1 (pino RX) como entrada
    PORTB = 0;  // limpar as portas que estão configuradas como saidas
    inicializa_RS232(1200,1);//modo de alta velocidade
    PEIE = 1;//habilita interrupção de perifericos do pic
    GIE = 1; //GIE: Global Interrupt Enable bit
    //imprime("Usando serial MPlab xc8 \n\r");
    //imprime("digite algo \n\r");
    for(;;)
    {
        imprime ("Hola Mundo!");
        __delay_ms(1000);
        //escreve ('\n');
        imprime ("I'm Alive and acoustic");
        //escreve ('\n');
        __delay_ms(1000);
        if(flag_interrupcao ==  1)
        {//tem dados para ler
         //imprime("digitaste:");
         escreve(caracter);
         flag_interrupcao = 0;
        }

    }//loop infinito
}


