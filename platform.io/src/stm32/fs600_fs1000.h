/*
 * Rad Pro
 * Bosean FS-600/FS-1000 pin definitions
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#define SYSCLK_FREQUENCY 8000000
#define AHB_FREQUENCY SYSCLK_FREQUENCY
#define APB1_FREQUENCY SYSCLK_FREQUENCY
#define TIM_FREQUENCY SYSCLK_FREQUENCY
#define DEADTIME_TIM_FREQUENCY SYSCLK_FREQUENCY

#define PAYLOAD_BASE 0x08000000
#define PAYLOAD_SIZE (0x8000 - 0x4)

#define PWR_EN_PORT GPIOA
#define PWR_EN_PIN GPIO11
#define PWR_BAT_PORT GPIOA
#define PWR_BAT_PIN GPIO6
#define PWR_BAT_CHANNEL 6
#define PWR_BAT_SCALE_FACTOR (2.0F / 3.0F)
#define PWR_CHRG_PORT GPIOB
#define PWR_CHRG_PIN GPIO9

#define TUBE_HV_PORT GPIOA
#define TUBE_HV_PIN GPIO4
#define TUBE_HV_AF GPIO_AF4
#define TUBE_HV_TIMER_RCC RCC_TIM14
#define TUBE_HV_TIMER TIM14
#define TUBE_HV_TIMER_CCMR TIM_CCMR1
#define TUBE_HV_TIMER_CCMR_MODE TIM_CCMR1_OC1M_PWM1
#define TUBE_HV_TIMER_CCER_CC TIM_CCER_CC1E
#define TUBE_HV_TIMER_CCR TIM_CCR1
#define TUBE_DET_PORT GPIOA
#define TUBE_DET_PIN GPIO2
#define TUBE_DET_PULLUP
#define TUBE_DET_EXTI EXTI2
#define TUBE_DET_EXTI_IRQ NVIC_EXTI2_3_IRQ
#define TUBE_DET_EXTICR_REG EXTI_EXTICR1
#define TUBE_DET_EXTICR_VALUE (EXTI_EXTICR_GPIOA << (2 * EXTI_EXTICR_FIELDSIZE))
#define TUBE_DET_IRQ_HANDLER exti2_3_isr
#define TUBE_DET_TIMER_LOW_RCC RCC_TIM3
#define TUBE_DET_TIMER_LOW TIM3
#define TUBE_DET_TIMER_HIGH TIM15
#define TUBE_DET_TIMER_HIGH_RCC RCC_TIM15

#define KEY_POWER_PORT GPIOB
#define KEY_POWER_PIN GPIO15
#define KEY_ENTER_PORT GPIOC
#define KEY_ENTER_PIN GPIO6
#define KEY_BACK_PORT GPIOC
#define KEY_BACK_PIN GPIO7
#define KEY_UP_PORT GPIOA
#define KEY_UP_PIN GPIO9
#define KEY_DOWN_PORT GPIOA
#define KEY_DOWN_PIN GPIO8

#define PULSE_LED_PORT GPIOA
#define PULSE_LED_PIN GPIO0
#define PULSE_LED_ACTIVE_LOW

#define LCD_RSTB_PORT GPIOB
#define LCD_RSTB_PIN GPIO2
#define LCD_CSB_PORT GPIOB
#define LCD_CSB_PIN GPIO1
#define LCD_A0_PORT GPIOB
#define LCD_A0_PIN GPIO10
#define LCD_SCLK_PORT GPIOB
#define LCD_SCLK_PIN GPIO11
#define LCD_SDA_PORT GPIOB
#define LCD_SDA_PIN GPIO12
#define LCD_BACKLIGHT_PORT GPIOB
#define LCD_BACKLIGHT_PIN GPIO0
#define LCD_BACKLIGHT_AF GPIO_AF2
#define LCD_BACKLIGHT_TIMER_RCC RCC_TIM1
#define LCD_BACKLIGHT_TIMER TIM1
#define LCD_BACKLIGHT_TIMER_CCMR TIM_CCMR1
#define LCD_BACKLIGHT_TIMER_CCMR_MODE TIM_CCMR1_OC2M_PWM1
#define LCD_BACKLIGHT_TIMER_CCER_CC TIM_CCER_CC2E
#define LCD_BACKLIGHT_TIMER_CCR TIM_CCR2
#define LCD_BACKLIGHT_PWM_FREQUENCY 1000
#define LCD_BACKLIGHT_PWM_PERIOD (TIM_FREQUENCY / LCD_BACKLIGHT_PWM_FREQUENCY)

#define BUZZ_PORT GPIOA
#define BUZZ_PIN GPIO10
#define BUZZ_PORT2 GPIOD
#define BUZZ_PIN2 GPIO0
#define BUZZ_ACTIVE_LOW

#define USART_RX_PORT GPIOB
#define USART_RX_PIN GPIO7
#define USART_RX_AF GPIO_AF0
#define USART_TX_PORT GPIOB
#define USART_TX_PIN GPIO6
#define USART_TX_AF GPIO_AF0
#define USART_RCC RCC_USART1
#define USART_INTERFACE USART1
#define USART_IRQ NVIC_USART1_IRQ
#define USART_IRQ_HANDLER usart1_isr
