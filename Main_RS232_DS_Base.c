/*
 * File:   Main_RS232_DSPIC.c
 * Author: mario
 *
 * Created on 22 de agosto de 2022, 08:31 PM
 */

#include <xc.h>
#include "RS232.h"
//Fosc = 7.37MHz Por Defecto
#define FCY 1842500
#include <libpic30.h>

// DSPIC30F4013 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FOSFPR = FRC             // Oscillator (Internal Fast RC (No change to Primary Osc Mode bits))
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define LED_CPU _LATD3



/*------------------------- Función de Interrupción Timer 1 ----------------*/
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void);

void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void);

void main(void) {
    /*------------------ Configuración de Pines Digital --------------------*/
    TRISD=0;
    LATD=0;
    _LATD9 = 1;
    /*------------------ Configuración del Timer 1 -------------------------*/
    PR1=7196;
    TMR1=0;
    _T1IF=0;
    T1CON = 0x8020;
    /*------------------ Configuración de RS232 ---------------------------*/
    Activar_RS232();
    /*------------------ Configuración de Interrupciones -------------------*/
    /**** Interrupción Timer 1 ****/
    _T1IE = 1;  //Habilitación Interrupción Timer1
    _T1IP = 7;  //Definición de Prioridad del Timer1
    _T1IF = 0;  //Inicializar la bandera de interrupción en 0
    
    __delay_ms(1000);
    _LATD9 = 0;
    char AUX;
    while(1){
        //AUX=U2RXREG;
        _LATD9 = 1;
        MensajeRS232("Hola Mundo\n");
        MensajeRS232(BufferR2);
        _LATD9 = 0;
        //Transmitir(AUX);
        __delay_ms(1000);
    }
}
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void){
    LED_CPU=LED_CPU^ 1; // Conmutar PinC13 LED CPU
    _T1IF=0;            // Reset de bandera de interrupción en Cero
}
void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void){
    Interrupcion_RS232();
}
