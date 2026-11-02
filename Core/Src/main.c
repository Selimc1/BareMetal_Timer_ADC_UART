#include "stm32f4xx.h"
#include <stdio.h>

void system_Init(void){
    // --- CLOCK AYARLARI ---
    RCC->AHB1ENR |= (1<<0); // GPIOA Clock
    RCC->AHB1ENR |= (1<<1); // GPIOB Clock
    RCC->APB1ENR |= (1<<17);// USART2 Clock
    RCC->APB2ENR |= (1<<8); // ADC1 Clock
    RCC->APB1ENR |= (1<<1); // TIM3 Clock

    // --- PA5 (STEP) ÇIKIŞI ---
    GPIOA->MODER |=  (1<<10);
    GPIOA->MODER &= ~(1<<11);
    GPIOA->BSRR   =  (1<<21); // Başlangıçta KESİNLİKLE LOW (0 Volt)

    // --- PB0 (LED) ÇIKIŞI ---
    GPIOB->MODER |=  (1<<0);
    GPIOB->MODER &= ~(1<<1);
    GPIOB->BSRR   =  (1<<16);

    // --- PA2 (UART TX) ---
    GPIOA->MODER &= ~(1<<4);
    GPIOA->MODER |=  (1<<5);
    GPIOA->AFR[0] |= (7<<8);

    // --- PA0 (ADC) ---
    GPIOA->MODER |=  (3<<0);
}

void USART2_Init(void){
    USART2->BRR = 0x008B; // 16 MHz clock, 115200 baud rate
    USART2->CR1 |=(1<<3); // TE
    USART2->CR1 |=(1<<13);// UE
}

void UART_SendString(char* str) {
    while(*str){
        while (!(USART2->SR & (1<<7)));
        USART2->DR = (*str & 0xFF);
        str++;
    }
}

void ADC1_Init(void){
    ADC1->CR2 |= (1<<0);
}

void TIM3_Init(void){
    TIM3->PSC = 16 - 1;
    TIM3->ARR = 1000 - 1; // Her 1 ms'de bir kesme
    TIM3->DIER |= (1<<0);
    NVIC_EnableIRQ(TIM3_IRQn);

    // TIMER'I BURADA DİREKT BAŞLATIYORUZ (Sistem açılır açılmaz kayıt başlar)
    TIM3->CR1 |= (1<<0);
}

// --- GLOBAL DEĞİŞKENLER ---
volatile uint16_t adc_value = 0;
volatile uint8_t data_ready = 0;
volatile uint32_t zaman_ms = 0; // Kronometre

void TIM3_IRQHandler(void){
    if(TIM3->SR & (1<<0)) {
       TIM3->SR &= ~(1<<0);

       zaman_ms++; // Her 1 milisaniyede zamanı artır

       // --- KRİTİK NOKTA: AKSİYONU SONRADAN BAŞLAT ---
       if(zaman_ms == 500){
           GPIOA->BSRR = (1<<5); // Tam 500. milisaniyede PA5'i HIGH yap (Step Ver)
       }

       // LED Toggle (İsteğe bağlı)
       if(zaman_ms % 500 == 0){
           GPIOB->ODR ^=(1<<0);
       }

       // ADC Okuma
       ADC1->CR2 |= (1<<30);
       while(!(ADC1->SR & (1<<1)));

       adc_value = ADC1->DR;
       data_ready = 1;
    }
}

int main(void){
    system_Init();
    USART2_Init();
    ADC1_Init();

    // Artık gecikme (for döngüsü) yok!
    // Timer Init direkt çalışıp kayda başlıyor.
    TIM3_Init();

    while(1){
        if(data_ready){
            data_ready = 0;
            char buffer[20];
            // \r\n ile Python arayüzüne veya Terminal'e şelale gibi akmasını sağlıyoruz
            sprintf(buffer, "%d\r\n", adc_value);
            UART_SendString(buffer);
        }
    }
}
