/*
 * sim800c_adapter.h
 *
 *  Created on: Apr 16, 2025
 *      Author: SOYLU
 */

#ifndef SIM800C_LIB_SIM800C_ADAPTER_H_
#define SIM800C_LIB_SIM800C_ADAPTER_H_

#ifndef SIM800C_ADAPTER_H_
#define SIM800C_ADAPTER_H_

#include <stdint.h>

int SIM800C_UART_Transmit_Adapter(void *huart, const uint8_t *data, uint16_t size, uint32_t timeout);
int SIM800C_UART_Receive_IT_Adapter(void *huart, uint8_t *data, uint16_t size);
uint32_t SIM800C_GetTick_Adapter(void);
void SIM800C_Delay_Adapter(uint32_t ms);

#endif


#endif /* SIM800C_LIB_SIM800C_ADAPTER_H_ */
