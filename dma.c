#include "dma.h"

unsigned int adcData[32] __attribute__((space(dma)));

void initDmaChannel3(void)
{

	//DMA4CON		= 0x;
	DMA3CONbits.CHEN = 0;	 // Disable channel
	DMA3CONbits.SIZE = 0;	 // Data transfer size (1=byte,0=word)
	DMA3CONbits.DIR = 0;	  // Transfer direction (1=read RAM,write to Periph. 0= read from periph, write to RAM)
	DMA3CONbits.HALF = 0;	 // Early block transfer complete interrupt (1=interrupt on half block transfer,0=int on full block transfer)
	DMA3CONbits.NULLW = 1;	// Null Data write to peripheral mode (Null data write to peripheral as well as write ro RAM, 0=normal)
	DMA3CONbits.AMODE = 0b10; // DMA channel operating mode	2,3=Peripheral Indirect Addressing mode, 1=Register Indirect without Post-Increment mode, 0=Register Indirect with Post-Increment mode
	DMA3CONbits.MODE = 0;	 // DMA channel opering mode select 0=Continuous, Ping-Pong modes disabled, 2=continuous, ping-pong

	DMA3REQbits.FORCE = 0;   // Force DMA Transfer (1=single DMA transfer,0=automatic initiated by DMA request)
	DMA3REQbits.IRQSEL = 13; // DMA Peripheral IRQ number select (ADC1)

	DMA3STA = (__builtin_dmaoffset(&(adcData[0]))); // start address of DMA RAM
	DMA3PAD = (volatile unsigned int)&ADCBUF0;		// address of peripheral sfr (0x0300)
	DMA3CNT = 2;									// we have 3 a2d  s/h channels for  measurement

	IFS2bits.DMA3IF = 0;  // Clear DMA interrupt
	IEC2bits.DMA3IE = 1;  // enable interrupt
	IPC9bits.DMA3IP = 6; // set priority of interrupt

	DMA3CONbits.CHEN = 1; // enable channel
}

void __attribute__((interrupt, auto_psv)) _DMA4Interrupt(void)
{
	IFS2bits.DMA3IF = 0; // Clear DMA interrupt
};

