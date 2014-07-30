/**
 * motors.h
 * Functions to initalize and control motor speeds
 */

#include "stm32f30x_tim.h"

/**
 * System clock runs at 72MHz
 * Set TIMER frequency at 2MHz
 * Prescaler = SYS_CLK / TIM_FREQ = 36
 * PWM frequency at 200 Hz
 * PWM period = TIM_FREQ / PWM_FREQ = 10000
 * PWM pulse length = 2000 ms
 */
#define SYS_CLK       (uint32_t)(72e6)
#define TIM_FREQ      (uint32_t)(2e6)
#define PWM_PRESCALER (uint32_t)(SYS_CLK / TIM_FREQ)
#define PWM_FREQ      (uint32_t)(200)
#define PWM_PERIOD    (uint32_t)((TIM_FREQ / PWM_FREQ))
#define PWM_PULSE     (uint32_t)(PWM_PERIOD / (1000.0/PWM_FREQ))

/**
 * Commonly used motor speeds
 * PWM pulse widths
 */
#define MOTOR_SPEED_MIN  (uint32_t)(1 * PWM_PULSE)
#define MOTOR_SPEED_HALF (uint32_t)(1.5 * PWM_PULSE)
#define MOTOR_SPEED_MAX  (uint32_t)(2 * PWM_PULSE)
#define MOTOR_SPEED_OFF  (uint32_t)(1 * PWM_PULSE)

/**
 * Identifiers for motors
 * LEFT FRONT   -   PC6  -  CH1
 * RIGHT FRONT  -   PC9  -  CH4
 * LEFT BACK    -   PC7  -  CH2
 * RIGHT BACK   -   PC8  -  CH3
 */
#define MOTOR_LEFT_FRONT    (uint8_t)(0x1)
#define MOTOR_RIGHT_FRONT   (uint8_t)(0x2)
#define MOTOR_LEFT_BACK     (uint8_t)(0x4)
#define MOTOR_RIGHT_BACK    (uint8_t)(0x8)

/**
 * Test if motor speed is within valid range
 */
#define IS_VALID_SPEED(SPEED) ((SPEED >= MOTOR_SPEED_MIN) && (SPEED <= MOTOR_SPEED_MAX))

/**
 * Convert percentage of full speed to PWM pulse width
 */
#define MOTOR_SPEED_PERCENTAGE(PERCENTAGE) (uint32_t)((PERCENTAGE * ((MOTOR_SPEED_MAX - MOTOR_SPEED_MIN) / 100)) + MOTOR_SPEED_MIN)

/**
 * Initialize timers for PWM control
 */
void motors_init(void);

/**
 * Set PWM duty cycle
 * motor - Select one or more motors to set:
 *         MOTOR_LEFT_FRONT
 *         MOTOR_RIGHT_FRONT
 *         MOTOR_LEFT_BACK
 *         MOTOR_RIGHT_BACK
 * speed - uint in range of [2000 - 4000]
 *         2000 = 0%   (1.0ms) -  motors off
 *         3000 = 50%  (1.5ms) -  half speed
 *         4000 = 100% (2.0ms) -  full speed
 */
void motors_set_speed(uint8_t motor, uint32_t speed);
