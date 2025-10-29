/*-------------------------------------------
功能：暖风机控制系统
版本：V1.0
日期：2025.10.29

芯片：CMS79F726
功能：
- 触摸按键控制（电源、摆头、模式、定时）
- 数码管状态显示
- 步进电机摆头控制
- 风扇PWM调速
- PTC加热器软启动
- 防倒开关安全检测
- 定时功能

引脚配置：
- RB0-3: 触摸按键 (TK15-12)
- RA0-2,RB4-5: 数码管显示
- RB7,RA7,RA6,RA5: 步进电机
- RB6: 防倒开关
- RA3: 风扇PWM (CCP1)
- RA4: PTC PWM (CCP2)
-------------------------------------------*/

#include <cms.h>

#include "config.h"

// 全局变量
SystemState sys_state = {0};
uint8_t motor_step = 0;
uint8_t motor_direction = 0;
uint8_t display_counter = 0;
uint8_t motor_counter = 0;
uint8_t ptc_startup_counter = 0;
volatile uint8_t touch_counter = 0; // 触摸按键扫描计数器

// 步进电机步序表
const uint8_t motor_steps[4] = {
    0x01, // A
    0x02, // B
    0x04, // C
    0x08  // D
};

// 函数声明
void system_init(void);
void key_scan(void);
void display_update(void);
void motor_control(void);
void fan_control(void);
void ptc_control(void);
void safety_check(void);
void timer_process(void);

void main(void) {
    asm("nop");
    asm("clrwdt");

    system_init();

    while (1) {
        asm("clrwdt");

        safety_check(); // 安全检测

        // 触摸按键主扫描 (每4ms调用一次)
        if (touch_counter >= 32) {
            touch_counter = 0;
            __CMS_CheckTouchKey(); // 触摸按键扫描
#if (REL_SENDER_ENABLE == 1)       // 调试宏定义是否为1
            REL_SenderLoop();      // 发码子程序
#endif
        }

        if (sys_state.safety) {
            key_scan();       // 按键扫描
            display_update(); // 显示更新
            motor_control();  // 电机控制
            fan_control();    // 风扇控制
            ptc_control();    // PTC控制
            timer_process();  // 定时处理
        } else {
            // 安全故障，关闭所有输出
            sys_state.power_on = 0;
            CCPR1L = 0; // 关闭风扇
            CCPR2L = 0; // 关闭PTC
        }
    }
}

/***********************************************
函数名称：system_init
函数功能：系统初始化
入口参数：无
出口参数：无
备注：
************************************************/
void system_init(void) {
    // 基本配置
    OPTION_REG = 0x01; // Timer0预分频1:4
    OSCCON = 0x71;     // 内部16MHz时钟

    // IO配置
    TRISA = 0x00; // RA全部输出
    TRISB = 0x0D; // RB0-3,RB6输入，其他输出
    TRISC = 0x03; // RC0-1为串口

    // 模拟功能关闭
    ANSEL0 = 0x00;
    ANSEL1 = 0x00;
    ANSEL2 = 0x00;

    // 上拉电阻配置
    WPUB = 0x0D; // RB0-3,RB6开启上拉

    // 触摸按键配置
    PAHEN = 0x00;
    PALEN = 0x00;
    PBHEN = 0x0F; // RB0-3高电平触摸开启
    PBLEN = 0x00;

    // 数码管驱动电流
    SEGCUR = 0x0F; // 最大驱动电流

    // PWM配置
    PWMCON = 0x56; // PWM1=16分频，PWM2=4分频，开启PWM1和PWM2

    // PWM1配置 (风扇控制)
    CCP1CON = 0x0C; // PWM模式
    PWM1CYC = 199;  // 周期800us
    CCPR1L = 0;     // 初始占空比0%

    // PWM2配置 (PTC控制)
    CCP2CON = 0x0C; // PWM模式
    PWM2CYC = 99;   // 周期100us
    CCPR2L = 0;     // 初始占空比0%

    // 触摸按键定时器配置 (Timer2) - 优先配置
    PIE1 = 0x02;   // 开启Timer2中断
    PR2 = 125;     // 16M下TMR2设置为125us中断
    T2CON = 5;     // 使能定时器2
    
    // 定时器中断配置
    TMR0 = 6;      // 1ms中断
    INTCON = 0xE0; // 开启全局中断、外设中断和Timer0中断

    // 初始状态
    sys_state.power_on = 0;
    sys_state.swing_on = 0;
    sys_state.mode = 0;
    sys_state.timer = 0;
    sys_state.safety = 1;

    PORTA = 0x00;
    PORTB = 0x00;
}

/***********************************************
函数名称：key_scan
函数功能：按键扫描处理
入口参数：无
出口参数：无
备注：
************************************************/
void key_scan(void) {
    static uint16_t KeyOldFlag = 0;
    uint16_t KeyOnOutput = (uint16_t)((_CMS_KeyFlag[1] << 8) | _CMS_KeyFlag[0]);

    if (KeyOnOutput) {
        if (KeyOnOutput != KeyOldFlag) {
            KeyOldFlag = KeyOnOutput; // 检测到按键

            // 电源键处理 (Key1)
            if (0x1 & KeyOnOutput) {
                sys_state.power_on = (sys_state.power_on == 0U) ? 1U : 0U;
                if (sys_state.power_on == 0U) {
                    sys_state.swing_on = 0U;
                    sys_state.mode = 0U;
                    sys_state.timer = 0U;
                }
            }
            // 摆头键处理 (Key2)
            else if (0x2 & KeyOnOutput && sys_state.power_on) {
                sys_state.swing_on = (sys_state.swing_on == 0U) ? 1U : 0U;
            }
            // 模式键处理 (Key3)
            else if (0x4 & KeyOnOutput && sys_state.power_on) {
                sys_state.mode++;
                if (sys_state.mode > 2U) {
                    sys_state.mode = 1U; // 循环：冷风->暖风->冷风
                }
            }
            // 定时键处理 (Key4)
            else if (0x8 & KeyOnOutput && sys_state.power_on) {
                sys_state.timer++;
                if (sys_state.timer > 15U) {
                    sys_state.timer = 0U; // 循环：0->1->2->...->15->0
                }
            }
        }
    } else {
        KeyOldFlag = 0;
    }
}

/***********************************************
函数名称：display_update
函数功能：数码管显示更新
入口参数：无
出口参数：无
备注：
************************************************/
void display_update(void) {
    static uint8_t display_index = 0;

    // 清除所有显示
    PORTA &= 0xF8; // 清除RA0-2
    PORTB &= 0xCF; // 清除RB4-5

    if (sys_state.power_on) {
        // 显示状态指示
        if (sys_state.mode == 1) {
            DISPLAY_A3 = 1; // 冷风模式
        } else if (sys_state.mode == 2) {
            DISPLAY_A3 = 1; // 暖风模式 (可以用不同的闪烁模式区分)
        }

        if (sys_state.swing_on) {
            if (motor_direction == 0) {
                DISPLAY_F2 = 1; // 上下摆头
            } else {
                DISPLAY_E1 = 1; // 左右摆头
            }
        }

        if (sys_state.timer > 0) {
            DISPLAY_A2 = 1; // 定时指示
        }
    }
}

/***********************************************
函数名称：motor_control
函数功能：步进电机控制
入口参数：无
出口参数：无
备注：
************************************************/
void motor_control(void) {
    static uint16_t step_count = 0;

    if (sys_state.swing_on && sys_state.power_on) {
        // 步进电机控制
        MOTOR_A = (motor_steps[motor_step] & 0x01U) ? 1 : 0;
        MOTOR_B = (motor_steps[motor_step] & 0x02U) ? 1 : 0;
        MOTOR_C = (motor_steps[motor_step] & 0x04U) ? 1 : 0;
        MOTOR_D = (motor_steps[motor_step] & 0x08U) ? 1 : 0;

        if (motor_direction == 0U) {
            motor_step++;
            if (motor_step >= 4U) {
                motor_step = 0;
            }
        } else {
            if (motor_step == 0U) {
                motor_step = 3;
            } else {
                motor_step--;
            }
        }

        step_count++;
        if (step_count >= 200U) { // 一圈后换向
            step_count = 0;
            motor_direction = (motor_direction == 0U) ? 1U : 0U;
        }
    } else {
        // 关闭电机
        MOTOR_A = 0;
        MOTOR_B = 0;
        MOTOR_C = 0;
        MOTOR_D = 0;
    }
}

/***********************************************
函数名称：fan_control
函数功能：风扇PWM控制
入口参数：无
出口参数：无
备注：
************************************************/
void fan_control(void) {
    if (sys_state.power_on && (sys_state.mode == 1 || sys_state.mode == 2)) {
        // 根据模式设置风扇速度
        if (sys_state.mode == 1) {
            CCPR1L = 50; // 冷风模式，中等速度
        } else {
            CCPR1L = 80; // 暖风模式，较高速度
        }
    } else {
        CCPR1L = 0; // 关闭风扇
    }
}

/***********************************************
函数名称：ptc_control
函数功能：PTC加热器控制
入口参数：无
出口参数：无
备注：软启动功能
************************************************/
void ptc_control(void) {
    if (sys_state.power_on && sys_state.mode == 2) {
        // 软启动：逐渐增加功率
        if (ptc_startup_counter < 100) {
            CCPR2L = ptc_startup_counter;
            ptc_startup_counter++;
        } else {
            CCPR2L = 100; // 满功率
        }
    } else {
        CCPR2L = 0; // 关闭PTC
        ptc_startup_counter = 0;
    }
}

/***********************************************
函数名称：safety_check
函数功能：安全检测
入口参数：无
出口参数：无
备注：
************************************************/
void safety_check(void) {
    sys_state.safety = (SAFETY_SW != 1) ? 1U : 0U; // 防倒开关检测（默认1）
}

/***********************************************
函数名称：timer_process
函数功能：定时处理
入口参数：无
出口参数：无
备注：
************************************************/
void timer_process(void) {
    static uint16_t hour_counter = 0;
    static unsigned long timer_counter = 0;

    if (sys_state.timer > 0U && sys_state.power_on) {
        timer_counter++;
        if (timer_counter >= 3600000UL) { // 1小时 (假设1ms中断)
            timer_counter = 0;
            hour_counter++;

            if (hour_counter >= (uint16_t)sys_state.timer) {
                // 定时到，关闭设备
                sys_state.power_on = 0U;
                sys_state.timer = 0U;
                hour_counter = 0;
            }
        }
    } else {
        hour_counter = 0;
        timer_counter = 0;
    }
}

/***********************************************
函数名称：Timer0_Isr
函数功能：定时器0中断服务程序
入口参数：无
出口参数：无
备注：1ms中断
************************************************/
void interrupt Timer0_Isr() {
    // Timer2中断处理 (125us) - 触摸按键扫描 (优先处理)
    if (TMR2IF) {
        TMR2IF = 0; // 清除中断标志
        touch_counter++;
        __CMS_GetTouchKeyValue(); // 触摸按键值获取
    }
    
    // Timer0中断处理 (1ms)
    if (T0IF) {
        TMR0 += 6; // 重装载定时器
        T0IF = 0;  // 清除中断标志

        // 显示刷新计数
        display_counter++;
        if (display_counter >= 5) {
            display_counter = 0;
            // 在主循环中处理显示更新
        }

        // 电机步进计数
        motor_counter++;
        if (motor_counter >= 50) { // 50ms步进一次
            motor_counter = 0;
            // 在主循环中处理电机控制
        }
    }
}