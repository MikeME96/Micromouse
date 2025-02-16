// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code Protection (General Segment Code protect is disabled)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Source Selection (Primary Oscillator (XT, HS, EC) with PLL)
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = HS              // Primary Oscillator Source (HS Crystal Oscillator Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 is general purpose digital I/O pin)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTSS1 = ON              // Enable Alternate SS1 pin bit (SS1A is selected as the I/O pin for SPI1)
#pragma config ALTQIO = OFF             // Enable Alternate QEI1 pin bit (QEA1, QEB1, INDX1 are selected as inputs to QEI1)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is disabled)

// FCMP
#pragma config HYST0 = HYST45           // Even Comparator Hysteresis Select (45 mV Hysteresis)
#pragma config CMPPOL0 = POL_FALL       // Comparator Hysteresis Polarity (for even numbered comparators) (Hysteresis is applied to falling edge)
#pragma config HYST1 = HYST45           // Odd Comparator Hysteresis Select (45 mV Hysteresis)
#pragma config CMPPOL1 = POL_FALL       // Comparator Hysteresis Polarity (for odd numbered comparators) (Hysteresis is applied to falling edge)


#include "xc.h"
#include"uart.h"
#include"stdio.h"
#include"string.h"
#include"p33FJ64GS608.h"
#include"dsp.h"
#include"stdlib.h"
#include"adc.h"
#include"dma.h"
#include "libpic30.h"
#include "maze.h"
long int an0, an1,an2,an3,an4,an5;
long int an1;
uint8_t print_buffer[256] = {0};
volatile int startExploreFlag = 0;  // Global variable
volatile int t = 100;

void __attribute__((interrupt, no_auto_psv)) _ADCP0Interrupt (void)
{
    
//    an0 = ADCBUF0; // Read AN0 conversion result
    an1 = ADCBUF1; // Read AN1 conversion result
    
    
    
//    sprintf(print_buffer, "\r read 1: %ld\t", an1);
//    send(print_buffer);
    // Interrupt Service Routine code goes here
    IFS6bits.ADCP0IF = 0; // Clear ADC Pair 0 Interrupt Flag
}

void __attribute__((interrupt, no_auto_psv)) _ADCP1Interrupt (void)
{
    
    
    an2 = ADCBUF2;
    an3 = ADCBUF3;
    
//    sprintf(print_buffer, "\r read 2: %d, read 3: %d\t", an2, an3);
//    send(print_buffer);
//    

    // Interrupt Service Routine code goes here
    IFS6bits.ADCP1IF = 0; // Clear ADC Pair 0 Interrupt Flag
}

void check_QEI(void) {
    // Check if QEI1 and QEI2 are enabled
    if (QEI1CONbits.QEIM != 0b111) {
        U1TXREG = 'E'; // Send 'E' if QEI1 is not enabled
    }
    if (QEI2CONbits.QEIM != 0b111) {
        U1TXREG = 'E'; // Send 'E' if QEI2 is not enabled
    }
    
    // Calculate speed and position for both motors
    calculate_speed('L');
    calculate_speed('R'); 
    calculate_position('L');
    calculate_position('R');
    
    // Print out the encoder counts and calculated distances for debugging
    char buffer[64];
    
    sprintf(buffer, "\rMotor 1 - Speed: %d, Distance: %ld\t", current_speed1, distance1);
    send(buffer); // Assuming you have a function to send a string over UART
    
    
    sprintf(buffer, "\rMotor 2 - Speed: %d, Distance: %ld\t", current_speed2, distance2);
    send(buffer); // Assuming you have a function to send a string over UART
}

void setup()
{
    PLLFBD = 48;            // M = 50 (PLLFBD + 2)
    CLKDIVbits.PLLPOST = 0; // N1 = 2 (output divided by 2)
    CLKDIVbits.PLLPRE = 0;  // N2 = 2 (input divided by 2)
}



int main(void) {
    
   

    setup();
    initIO();
    init_PWM();
    initTimer1(6250); //6250 gives 4.2ms timer interrupt with 64 prescaler
    initTimer2(2600); //2600 gives 1.8ms timer interrupt with 64 prescaler

    initUART();
    setupADC1();
    startADC1();
    startTimer1(); 
    startTimer2();
    init_QEI();
    
   
    
    setMazeBorders();

    
    __builtin_enable_interrupts();
 
    
////////////////////////////////////////////////////////////////
///////////////////////TESTING STUFF////////////////////////////
///////////////////////////////////////////////////////////////

    char buffer[64];

///////////////////////////////////////MAZE SOLVING TESTING GROUND/////////////////////////////////////////
    
__delay_ms(2000);
Direction current_direction = NORTH; 
int current_X = 0;
int current_Y = 0;

setWalls(current_X, current_Y, current_direction, getRight(), getLeft(), getFront());

int x_target = 2;
int y_target = 1;

//////////////////////////////////////// EXPLORATION TEST /////////////////////////////////////////////////////////
//explore(current_X, current_Y, current_direction, x_target, y_target);
//printMazeWallBits();
//initializeAndStartExplore();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (1) {

        
        if (startExploreFlag == 1) {
            initializeAndStartExplore();  // Start exploration when the flag is set
             sprintf(buffer, "\r Front wall : %d, Right wall : %d, Left wall :%d\t", frontWall(), rightWall(), leftWall());
            send(buffer);
            startExploreFlag = 0;         // Reset the flag after starting exploration
        }
    }
    return 0;
}
