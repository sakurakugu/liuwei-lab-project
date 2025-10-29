/*-------------------------------------------
版本：V1.0
日期：2025.10.29
-------------------------------------------*/

#ifndef _WARM_AIR_BLOWER_H_
#define _WARM_AIR_BLOWER_H_

#include <stdint.h>
#include "touch_key\Touch_Kscan_Library.h"
#include "touch_key\REL_Sender.h"

// 系统时钟配置
#define SYSTEM_CLOCK    16000000    // 16MHz
#define TIMER_1MS       6           // 1ms

// 触摸按键定义
#define KEY_POWER   (_CMS_KeyFlag[0] & 0x01)    // 电源键 TK15 Key1
#define KEY_SWING   (_CMS_KeyFlag[0] & 0x02)    // 摆头键 TK14 Key2  
#define KEY_MODE    (_CMS_KeyFlag[0] & 0x04)    // 模式键 TK13 Key3
#define KEY_TIMER   (_CMS_KeyFlag[0] & 0x08)    // 定时键 TK12 Key4

#define DISPLAY_A1  RA0     // PIN1 - A1/B1/C1/D1
#define DISPLAY_A2  RA1     // PIN2 - A2/B2/C2/D2
#define DISPLAY_A3  RA2     // PIN3 - A3/B3/C3/D3
#define DISPLAY_E1  RB4     // PIN4 - E1/F1/G1/E2
#define DISPLAY_F2  RB5     // PIN5 - F2/G2/E3/F3

#define MOTOR_A     RB7     // PIN6 - A??
#define MOTOR_B     RA7     // PIN7 - B??
#define MOTOR_C     RA6     // PIN8 - C??
#define MOTOR_D     RA5     // PIN9 - D??

#define SAFETY_SW   RB6     // PIN10 - 防倒开关
#define FAN_PWM     RA3     // PIN11 - 风扇PWM (CCP1)
#define PTC_PWM     RA4     // PIN12 - PTC加热器PWM (CCP2)

// 模式定义
#define MODE_OFF        0   // 关闭
#define MODE_COLD       1   // 冷风模式
#define MODE_WARM       2   // 热风模式

// 定时定义
#define TIMER_OFF       0   // 关闭
#define TIMER_MAX       15  // 最大定时时间15分钟

// 按键防抖时间
#define KEY_DEBOUNCE    20  // 20ms防抖

// 步进电机定义
#define MOTOR_STEPS     4   // 4个步骤
#define MOTOR_CYCLE     200 // 一圈200步

// PWM占空比定义
#define FAN_PWM_COLD    50  // 冷风PWM占空比
#define FAN_PWM_WARM    80  // 热风PWM占空比
#define PTC_PWM_MAX     100 // PTC加热器PWM占空比
#define PTC_STARTUP_TIME 100 // PTC软启动时间100ms

// 系统状态定义
typedef struct {
    uint8_t power_on : 1;     // 电源状态
    uint8_t swing_on : 1;     // 摆头状态
    uint8_t mode : 2;         // 模式状态
    uint8_t timer : 4;        // 定时状态
    uint8_t safety : 1;       // 防倒开关状态
} SystemState;

// 显示状态定义
typedef struct {
    uint8_t buffer[5];        // 显示缓冲区
    uint8_t index;            // 当前显示位置
    uint8_t blink_count;      // 闪烁计数器
} DisplayState;

// 电机状态定义
typedef struct {
    uint8_t step;             // 当前电机步骤
    uint8_t direction;        // 电机方向
    uint16_t step_count;      // 电机已走步数
    uint8_t speed;            // 电机速度
} MotorState;

// 系统状态外部变量
extern SystemState sys_state;
extern DisplayState display_state;
extern MotorState motor_state;
extern uint16_t timer_counter;
extern uint8_t ptc_startup_counter;

// 函数声明
void system_init(void);
void key_scan(void);
void display_update(void);
void motor_control(void);
void fan_control(void);
void ptc_control(void);
void safety_check(void);
void timer_process(void);

// 延时函数
void delay_ms(uint16_t ms);
void set_display_digit(uint8_t pos, uint8_t value);
void motor_step_forward(void);
void motor_step_backward(void);

#endif // _WARM_AIR_BLOWER_H_