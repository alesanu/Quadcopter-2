/**
 * stm32f30x_it.c: Manage interrupt handlers
 */

#include "stm32f30x_it.h"
#include "controls.h"
#include <string.h>

int i;
extern uint8_t enabled;
static __IO uint32_t TimingDelay;
uint16_t capture = 0;

uint8_t pid_run_flag = 0;

////////////////////////////////////////////////////////////////////////////////
//    Error Handlers
////////////////////////////////////////////////////////////////////////////////

void ErrorLoop(Led_TypeDef led) {
  int i = 0;
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(led);

  while (1) {
    for(i = 0; i < 0x7FFFF; i++);
    STM_EVAL_LEDToggle(LED3);
    STM_EVAL_LEDToggle(led);
  }
}

/**
 * Set LED3 and LED10 on Hard Fault
 */
void HardFault_Handler(void) {
  /* Go to infinite loop when Hard Fault exception occurs */
  ErrorLoop(LED10);
}

/**
 * Set LED3 and LED9
 */
void UsageFault_Handler(void) {
  ErrorLoop(LED9);
}

/**
 * Set LED3 and LED8
 */
void BusFault_Handler(void) {
  ErrorLoop(LED8);
}

/**
 * Set LED3 and LED7
 */
void MemMang_Hander(void) {
  ErrorLoop(LED7);
}

////////////////////////////////////////////////////////////////////////////////
//    User Handlers
////////////////////////////////////////////////////////////////////////////////

/**
 * User Button
 */
void EXTI0_IRQHandler(void) {
  if ((EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) == SET) && (STM_EVAL_PBGetState(BUTTON_USER) != RESET)) {
    /* Delay */
    for (i = 0; i < 0x7FFFF; i++);

    while (STM_EVAL_PBGetState(BUTTON_USER) != RESET);

    for (i = 0; i < 0x7FFFF; i++);

#ifndef DEBUG
    if (enabled)
      enabled = 0;
    else
      enabled = 1;
#endif

    EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);

  }
}

/**
 * Wait for amount of milliseconds
 * nTime - time to wait in milliseconds
 */
void Delay(__IO uint32_t nTime) {
  TimingDelay = nTime;
  while (TimingDelay != 0);
}

/**
 * Delay Decrementer
 */
void TimingDelay_Decrement(void) {
  if (TimingDelay != 0x00) {
    TimingDelay--;
  }
}

/**
 * SysTick Handler
 */
void SysTick_Handler(void) {
  TimingDelay_Decrement();
}

/**
 * USART3 Interrupt handler
 */
void USART3_IRQHandler(void) {
  if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET) {
    static uint8_t cnt = 0;
    char t = USART_ReceiveData(USART3);
    if ((t != '\n') && (cnt < CONTROL_MSG_SIZE)) {
      command_bytes[cnt] = t;
      cnt++;
    } else {
      command_valid = 0x01;
      // Convert received bytes to command
      controls_format(command_bytes, &command);
      // Reset command bytes
      memset(command_bytes, 0, CONTROL_MSG_SIZE);
      cnt = 0;
    }
  }
}

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    if (pid_run_flag)
      STM_EVAL_LEDOn(LED10);
    pid_run_flag = 1;
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}
