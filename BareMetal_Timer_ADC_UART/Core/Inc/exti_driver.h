#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

#include "stm32f4xx.h"
#include <stdint.h>

#define EXTI_PORT_A 0
#define EXTI_PORT_B 1
#define EXTI_PORT_C 2
#define EXTI_PORT_D 3

#define EXTI_EDGE_RISING 0
#define EXTI_EDGE_FALLING 1 
#define EXTI_EDGE_BOTH 2

typedef struct{
	uint8_t port;
	uint8_t pin;
	uint8_t edge;
	void(*callback)(void);
}EXTI_Config_t;

void EXTI_Init(EXTI_Config_t *cfg);

#endif