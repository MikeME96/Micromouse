#include "xc.h"
#include "gpio.h"
#include "dma.h"
#include <stdio.h>
#include "qei.h"
#include "control.h"
#include "pwm.h"
void init_PWM()
{
    /*~~~~~~~~~~~~~~~~~~~~~~~ PWM1 Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    
	
    // Disable the PWM Module for configuration
        PTCON    = 0x0000;
        PTCON2    = 0x0000;
        
        PTPER    = 0x07ff;
        PWMCON1    = 0x0000;
        PWMCON2    = 0x0000;

        IOCON1    = 0x8c00;
        IOCON2    = 0x8c00;

        
        TRISEbits.TRISE1      = 0;        // PWM1H Motor_1
        TRISEbits.TRISE3      = 0;        // PWM2H Motor_2
    
    
    PTPER = 2000; 
 
    
    PDC1 = 0; 
    PDC2 = 0;

    // Enable the PWM Module
    PTCON    = 0x8000;
    PTCON2    = 0x8000;
    
    
}

void drive_motor(char motor, int dutycycle)
{
    if (dutycycle > 0) {
        drive_motor_backward(motor, dutycycle);
    } else if (dutycycle < 0) {
        drive_motor_forward(motor, -dutycycle);
    } else {
        fast_stop_motor(motor);
    }
}

void drive_motor_forward(char motor, int dutycycle)
{
    if (motor == 'R'){
        HBRIDGE1 = 1;
        //HBRIDGE2 = 1;
//        sendNameValue("val", HBRIDGE1);
        PDC1 = dutycycle;
    } else if (motor == 'L'){
        //HBRIDGE1= 0;
        HBRIDGE2 = 0;
        PDC2 = dutycycle;
    }
}

void drive_motor_backward(char motor, int dutycycle)
{
    if (motor == 'R'){
        HBRIDGE1 = 0;
        //HBRIDGE2 = 0;
//        sendNameValue("val", HBRIDGE1);
        PDC1 = dutycycle;
    } else if (motor == 'L'){
        //HBRIDGE3 = 1;
        HBRIDGE2 = 1;
        PDC2 = dutycycle;
    }
}

void fast_stop_motor(char motor)
{
    if (motor == 'R'){
        PDC1=0;
    } else if (motor == 'L'){
        PDC2=0;
    }
}