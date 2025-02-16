
#include "uart.h"
#include "maze.h"
#include "libpic30.h"
#include"stdio.h"
#include"string.h"
#include"p33FJ64GS608.h"
#include"dsp.h"
#include"stdlib.h"
#include"adc.h"

extern volatile int startExploreFlag;  // Global variable
extern volatile int t;
void initUART()
{
    IPC3bits.U1TXIP = 5;
    TRISFbits.TRISF3 = 0;   // Set pin RF3 as Output
    TRISFbits.TRISF2 = 1;   // Set pin RF2 as Input
    U1MODEbits.STSEL = 0;   // 1-Stop bit
    U1MODEbits.PDSEL = 0;   // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0;   // Auto-Baud disabled
    U1MODEbits.BRGH = 0;    // Standard-Speed mode
    U1BRG = 299;//25;         // Baud Rate setting for BAUDRATE
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    
    U1STAbits.URXISEL = 0; // Interrupt after one RX character is received;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    IPC2bits.U1RXIP = 5;
    
    IEC0bits.U1TXIE = 1;   // Enable UART TX interrupt
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1;   // Enable UART TX
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    DELAY_105uS;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; 
}

/**
 * 
 * @param msg sends this message up to length 4
 */
void send(char msg[]){
    int i = 0, n = strlen(msg);
    while(i < n){
        if(!U1STAbits.UTXBF){
            U1TXREG = msg[i];
            i++;
        }
    }
    int send = 1;
    while(send){
        if(!U1STAbits.UTXBF){
            U1TXREG = '\n';
            send = 0;
        }
    }
}

void sendNameValue(char name[], int value){
    int i = 0, n = strlen(name);
    while(i < n){
        if(!U1STAbits.UTXBF){
            U1TXREG = name[i];
            i++;
        }
    }
    while(U1STAbits.UTXBF)
        ;
    U1TXREG = ' ';
    char buffer[5];
    sprintf(buffer, " %d ", value);
    i = 0, n = strlen(buffer);
    while(i < n){
        if(!U1STAbits.UTXBF){
            U1TXREG = buffer[i];
            i++;
        }
    }
    while(U1STAbits.UTXBF)
        ;
    U1TXREG = '\n';
}
void uart_Write(long int smth){
    if (!U1STAbits.UTXBF){
        U1TXREG = smth;
    }
}

//=================================
/* UART1 Receive Interrupt Service Routine */

void __attribute__((__interrupt__,auto_psv)) _U1RXInterrupt(void)
{
    char receivedChar;
    IFS0bits.U1RXIF = 0; 
    receivedChar = U1RXREG;
//    if (receivedChar == 'T') {
//        performDriveToTarget();
//    } else 
        if (receivedChar == 'S') {
            startExploreFlag = 1;  // Set flag when 'S' is pressed
        }
        if(receivedChar == 'V'){ // This was the last change
            vel_control_left.kp = 20;
            vel_control_right.kp = 20;
            t = 50;
            startExploreFlag = 1;
        }
//    }
    U1RXREG = 0x00;
    while(U1STAbits.UTXBF);
    U1TXREG = receivedChar;
    
} 


void performDriveToTarget() {
    go_one_cell();
}
void initializeAndStartExplore(){
    setMazeBorders();
    Direction current_direction = NORTH; 
    int current_X = 0;
    int current_Y = 0;
    int x_target = 2;
    int y_target = 1;
    setWalls(current_X, current_Y, current_direction, getRight(), getLeft(), getFront());
    __delay_ms(2000);
    explore(current_X, current_Y, current_direction, x_target, y_target);
}

