#include "gpio.h"

void initIO()
{
    int i;

    TRISEbits.TRISE1      = 0;		// PWM1H Motor_1  
	TRISEbits.TRISE3      = 0;		// PWM2H Motor_2


/***************************************************/		
		
    TRISEbits.TRISE0		= 0;		// dir_1
    TRISEbits.TRISE2		= 0;		// dir_2
    TRISEbits.TRISE4		= 0;		// M_En

    PORTEbits.RE0 = 0;
    PORTEbits.RE2 = 0;		

    PORTBbits.RB8 = 0;
    PORTBbits.RB9 = 0;
    PORTBbits.RB10 = 0;
    PORTBbits.RB11 = 0;
    PORTBbits.RB12 = 0;
    PORTBbits.RB13 = 0;
    
    TRISEbits.TRISE2 = 0;  // HBRIDGE1
    TRISEbits.TRISE0 = 0; // HBRIDGE2
    
    TRISDbits.TRISD11 = 1;   // ch_A1
    TRISDbits.TRISD0 = 1;    // ch_B1
    TRISDbits.TRISD12 = 1;   // ch_A2
    TRISGbits.TRISG1 = 1;    // ch_B2
    
    //sensor
    TRISBbits.TRISB0	= 1; 		// Distance sensor_0
    TRISBbits.TRISB1	= 1; 		// Distance sensor_1
    TRISBbits.TRISB2	= 1; 		// Distance sensor_2
    TRISBbits.TRISB3	= 1; 		// Distance sensor_3
    TRISBbits.TRISB4	= 1; 		// Distance sensor_4
    TRISBbits.TRISB5	= 1; 		// Distance sensor_5
// Set up remappable pins
    __builtin_write_OSCCONL(OSCCON & 0xbf); // Clear bit 6 (unlock, they are usually write protected)

    

    __builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS registers (lock again!)

    for (i = 0; i < 30000; i++)
        ; // Short delay
}
