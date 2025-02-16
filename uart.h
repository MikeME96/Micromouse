#ifndef UART_H
#define UART_H
#include "xc.h"
#include "qei.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FCY 50000000UL //50MHZ
#define BAUDRATE 9600
#define BRGVAL ((FCY / BAUDRATE) / 16) - 1
//unsigned int i;
#define DELAY_105uS                \
    asm volatile("REPEAT, #4201"); \
    Nop(); // 105uS delay

void initUART();
void send(char msg[]);
void sendNameValue(char name[], int value);

#endif /* GPIO_H */