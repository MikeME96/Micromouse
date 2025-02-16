
#include "qei.h"
#include "xc.h"
#include "uart.h"
#include "stdio.h"

//Motor no. 1 is the LEFT motor
long int longpos1 = 0; // Initialize long position count overflow variable
int current_speed1 = 0; // Initialize variable in which the current speed is stored
long int old_count1 = 0;
long int new_count1 = 0;

//Motor no. 2 is the RIGHT motor
long int longpos2 = 0; // Initialize long position count overflow variable
int current_speed2 = 0; // Initialize variable in which the current speed is stored
long int old_count2 = 0;
long int new_count2 = 0;

long int current_pos1 = 0;
long int current_pos2 = 0;

//distance in 10 micrometer 
// 1cm is 1000 of this 
long int distance1 = 0;
long int distance2 = 0;

void init_QEI(void)
{
    // Configure QEI module
    
    QEI1CONbits.CNTERR = 0; 
    QEI2CONbits.CNTERR = 0;
   
    QEI1CONbits.QEISIDL = 1; 
    QEI2CONbits.QEISIDL = 1;
    
    QEI1CONbits.UPDN = 0; 
    QEI2CONbits.UPDN = 0; 

  
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111;

    //SWPAB: Phase A and Phase B Input Swap Select bit
    QEI1CONbits.SWPAB = 1; // Do not swap the polarity of phase comparison
    QEI2CONbits.SWPAB = 0;

    //PCDOUT: Position Counter Direction State Output Enable bit
    QEI1CONbits.PCDOUT = 0; // Disable counter direction pin (normal I/O operation)
    QEI2CONbits.PCDOUT = 0;

    // TQGATE: Timer Gated Time Accumulation Enable bit
    QEI1CONbits.TQGATE = 0;
    QEI2CONbits.TQGATE = 0;

    //POSRES: Position Counter Reset Enable bit
    QEI1CONbits.POSRES = 0; // index pulse does not reset POSCNT
    QEI2CONbits.POSRES = 0;

    //TQCS: Timer Clock Source Select bitTQCS: Timer Clock Source Select bit
    QEI1CONbits.TQCS = 0; // internal clock source (Tcy)
    QEI2CONbits.TQCS = 0;

    DFLT1CONbits.QEOUT = 0; // disable digital filters bit 7
    DFLT2CONbits.QEOUT = 0;

    // set initial counter value and maximum range
    MAX1CNT = 0xffff; // set the highest possible time out
    POS1CNT = 0; // set POSCNT into middle of range
    MAX2CNT = 0xffff;
    POS2CNT = 0;

    // Configure Interrupt controller for QEI 1 - first motor
    IFS3bits.QEI1IF = 0;  // clear interrupt flag
    IEC3bits.QEI1IE = 1;  // enable QEI interrupt
    IPC14bits.QEI1IP = 6; // set QEI interrupt priority

    // Configure Interrupt controller for QEI 2 - second motor
    IFS4bits.QEI2IF = 0;  // clear interrupt flag
    IEC4bits.QEI2IE = 1;  // enable QEI interrupt
    IPC18bits.QEI2IP = 6; // set QEI interrupt priority
    
}

void calculate_speed(char motor){
    char buffer[64];
    // calculate current speed. 
    
    if(motor == 'L'){
        GET_ENCODER_VALUE_1(new_count1);
        current_speed1 = (new_count1 - old_count1);//*FCY; // gives speed in (encoder counts)/second
        old_count1 = new_count1;
        
    
//        sprintf(buffer, " \r Speed Left : %d\t", current_speed1);
//        send(buffer); // Assuming you have a function to send a string over UART
        
    } else if (motor == 'R'){
        GET_ENCODER_VALUE_2(new_count2);
        current_speed2 = (new_count2 - old_count2);//*FCY; // gives speed in (encoder counts)/second;
        old_count2 = new_count2;
//        sprintf(buffer, " \r Speed Right : %d\t", current_speed1);
//        send(buffer); // Assuming you have a function to send a string over UART
    }
}

void calculate_position(char motor){
    char buffer[64];
    if(motor == 'L'){
        GET_ENCODER_VALUE_1(current_pos1);
        distance1 = current_pos1* DISTANCE_PER_CNT;
//        sprintf(buffer, " \r Pos Left : %d\t", GET_ENCODER_VALUE_1(current_pos1));
//        send(buffer); // Assuming you have a function to send a string over UART
    } else if(motor == 'R'){
        GET_ENCODER_VALUE_2(current_pos2);
        distance2 = current_pos2* DISTANCE_PER_CNT;
//        sprintf(buffer, " \r Pos Right : %d\t", distance2);
//        send(buffer); // Assuming you have a function to send a string over UART
        }
}

// interrupt service routine that resets the position counter for the QEI 1
void __attribute__((interrupt, no_auto_psv)) _QEI1Interrupt(void)
{
    IFS3bits.QEI1IF = 0;  // clear interrupt flag
    //less than half of maxcount 32768
    if (POS1CNT < 0x7fff) {
//      U1TXREG = 'o'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos1 += 0x10000; //overflow condition caused interrupt
    } else {
//      U1TXREG = 'u'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos1-= 0x10000; //underflow condition caused interrupt
    }
        
}

// interrupt service routine that resets the position counter for the QEI 2
void __attribute__((interrupt, no_auto_psv)) _QEI2Interrupt(void)
{
    IFS4bits.QEI2IF = 0;  // clear interrupt flag
    
    if (POS2CNT < 0x7fff) {
//      U1TXREG = 'O'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos2 += 0x10000; //overflow condition caused interrupt
    } else {
//      U1TXREG = 'U'; // Just for debugging purposes
      // Saving count information in long variable in case of over/underflow
      longpos2 -= 0x10000; //underflow condition caused interrupt
    }
        
}

