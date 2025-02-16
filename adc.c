#include "adc.h"
#include <xc.h>

void startADC1(void)
{
        int i;

    ADCONbits.ADON = 1; //set on-bit
    
}

void setupADC1()
{

ADCONbits.ADON = 0; // Disable ADC Module    
ADCONbits.FORM = 1; // Output in Integer Format
ADCONbits.EIE = 1; // Enable Early Interrupt
ADCONbits.ORDER = 0; // Normal Order of Conversion
ADCONbits.SEQSAMP = 0; // Simultaneous Sampling
ADCONbits.ASYNCSAMP = 1; // Asynchronous Sampling
ADCONbits.SLOWCLK = 0; // High Frequency Clock Input
ADCONbits.ADCS = 6; // Clock Divider Selection
ADCPC0bits.TRGSRC0=0b01100; // Timer 1 period match
ADCPC0bits.TRGSRC1=0b00100; // Timer 2 period match
ADPCFGbits.PCFG0 = 0; // AN0 is configured as analog input
ADPCFGbits.PCFG1 = 0; // AN1 is configured as analog input
ADPCFGbits.PCFG2 = 0; // AN2 is configured as analog input
ADPCFGbits.PCFG3 = 0; // AN3 is configured as analog input
ADPCFGbits.PCFG4 = 0; // AN3 is configured as analog input
ADPCFGbits.PCFG5 = 0; // AN3 is configured as analog input

IPC27bits.ADCP0IP = 0x01; // Set ADC Pair 0 Interrupt Priority (Level 1)
IFS6bits.ADCP0IF = 0; // Clear ADC Pair 0 Interrupt Flag
IEC6bits.ADCP0IE = 1; // Enable ADC Pair 0 Interrupt

IPC27bits.ADCP1IP = 0x02; // Set ADC Pair 1 Interrupt Priority (Level 2)
IFS6bits.ADCP1IF = 0; // Clear ADC Pair 1 Interrupt Flag
IEC6bits.ADCP1IE = 1; // Enable ADC Pair 1 Interrupt
//
//IPC28bits.ADCP2IP = 0x03; 	// Set ADC Pair 2 Interrupt Priority 4-5
//IFS7bits.ADCP2IF = 0; 		// Clear ADC Pair 1 Interrupt Flag
//IEC7bits.ADCP2IE = 1; 		// Enable ADC Pair 1 Interrupt
//
//IPC28bits.ADCP3IP = 0x04; 	// Set ADC Pair 3 Interrupt Priority 6-7
//IFS7bits.ADCP3IF = 0; 		// Clear ADC Pair 1 Interrupt Flag
//IEC7bits.ADCP3IE = 1; 		// Enable ADC Pair 1 Interrupt


/* Example code for ADC Pair 0 ISR*/


    

}
