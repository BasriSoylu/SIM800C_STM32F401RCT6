/*
 * sim800c_adapter.c
 *
 *  Created on: Apr 16, 2025
 *      Author: SOYLU
 */

#include "sim800c.h"
#include "stm32f4xx_hal.h"

/*
 * Bu kütüphanedeki amaç her platforma uyumluluk sağlamasıdır. Eğer bu kütüphane düzgün kullanılır ise her mikrodenetleyicide sadece UART
 * protokolü fonksiyonları kullanılarak kütüphaneyi hızlıca kullanabilriz. Alt kısımda bulunan MICROCONTROLLER makrosunu kullanarak
 * mikrodenetleyicinizi seçebilirsiniz eğer mikrodenetleyiciniz listede yok ise aşşağıdaki _Adapter fonksiyonlarınızı güncelleyerek
 * Aynı sayıda ve gerekli parametreleri fonksiyonunuza verecek şekilde kullanarak bukütüphaneyi kolaylıkla gullanavilirsiniz.
 */


/*   !> Hangi mikrodenetleyici kullanılıyorsa ona karşılık gelen değer seçilmeli.
 *   -> STM32 -> 1
 *   -> NXP   -> 2
 */
#define MICROCONTROLLER 1




int SIM800C_UART_Transmit_Adapter(void *huart, const uint8_t *data, uint16_t size, uint32_t timeout){

#if (MICROCONTROLLER == 1)
    return HAL_UART_Transmit((UART_HandleTypeDef *)huart, (uint8_t *)data, size, timeout);
#endif

#if (MICROCONTROLLER == 2)
    return 0;
#endif

}

int SIM800C_UART_Receive_IT_Adapter(void *huart, uint8_t *data, uint16_t size){

#if (MICROCONTROLLER == 1)
    return HAL_UART_Receive_IT((UART_HandleTypeDef *)huart, data, size);
#endif

#if (MICROCONTROLLER == 2)
    return 0;
#endif
}

uint32_t SIM800C_GetTick_Adapter(void){

#if (MICROCONTROLLER == 1)
    return HAL_GetTick();
#endif

#if (MICROCONTROLLER == 2)
    return 0;
#endif

}

void SIM800C_Delay_Adapter(uint32_t ms){

#if (MICROCONTROLLER == 1)
    HAL_Delay(ms);
#endif


#if (MICROCONTROLLER == 2)
    return 0;
#endif

}

