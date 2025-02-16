
#include "timer.h"
#include "gpio.h"
#include <xc.h>
#include "qei.h"
#include "control.h"
#include "uart.h"
#include "dma.h"

char buffer3[64];
void initTimer1(unsigned int period)
{
    T1CON = 0;              // ensure Timer 1 is in reset state
    T1CONbits.TGATE = 0;    // gated time accumulation disabled
    T1CONbits.TCKPS = 0b10; // FCY divide by 64: tick = 1.6us
    T1CONbits.TCS = 0;      // select internal FCY clock source
    TMR1 = 0;
    PR1 = period - 1;  // set Timer 1 period register ()
    IFS0bits.T1IF = 0; // reset Timer 1 interrupt flag
    IPC0bits.T1IP = 5; // set Timer1 interrupt priority level to 4
    IEC0bits.T1IE = 1; // enable Timer 1 interrupt
    T1CONbits.TON = 0; // leave timer disabled initially
}

void startTimer1(void)
{
    T1CONbits.TON = 1; //
}

void stopTimer1(void)
{
    T1CONbits.TON = 0; //
}

void initTimer2(unsigned int period)
{
    
    T2CON = 0;              // ensure Timer 2 is in reset state
    T2CONbits.T32 = 0;      // Run Timer 2 in 16 bit instead of 32 bit
    T2CONbits.TGATE = 0;    // gated time accumulation disabled
    T2CONbits.TCKPS = 0b10; // FCY divide by 64: tick = 1.6us
    T1CONbits.TCS = 0;      // select internal FCY clock source
    TMR2 = 0;               // Just setting initial timer value to 0
    
    PR2 = period - 1;  // set Timer 2 period register ()
    IFS0bits.T2IF = 0; // reset Timer 2 interrupt flag
    IPC1bits.T2IP = 6; // set Timer2 interrupt priority level to 5
    IEC0bits.T2IE = 1; // enable Timer 2 interrupt
    T2CONbits.TON = 0; // leave timer disabled initially
}

void startTimer2(void)
{
    T2CONbits.TON = 1; //
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    // this gets set the first time it is called and then never again
    static int count = 0;

    IFS0bits.T1IF = 0; // reset Timer 1 interrupt flag

    
    if (count < 9)
    {
        count++;
    }
    else
    {
       
        count = 0;
    }
}

// Timer that is there to constantly update the current speed of the motors
// atm it's just one motor though.
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{

    IFS0bits.T2IF = 0; // reset Timer 2 interrupt flag
    calculate_position('L');
    calculate_position('R');
    calculate_speed('L'); // Call function from qei.c to calculate current speed
    calculate_speed('R'); // Call function from qei.c to calculate current speed2

    evaluate_movement(); // Call function from control.c to evaluate completion of movement
    
 
    
}
