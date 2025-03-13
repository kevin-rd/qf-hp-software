#pragma once

#include <Arduino.h>

typedef enum
{
    UI_STYLE_QFTEK,    // 启凡科创风格
    UI_STYLE_ZHAOYANG, // 找羊风格
} ui_style_t;          // 风格模式

typedef enum
{
    TEMP_MODE_CURVE,    // 回流焊模式
    TEMP_MODE_CONSTANT, // 恒温模式

} temp_mode_t;

typedef enum
{
    ENC_ROTATION_CW,
    ENC_ROTATION_CCW,
} enc_rotation_t; // 风格模式

typedef struct
{
    int16_t pwm_temp_buf;         // 目标温度值
    int16_t pwm_temp_mode1_time;  // 恒温模式定时时长
    int16_t curve_temp_buf[4];    // 回流曲线参数
    uint16_t adc_hotbed_max_temp; // 热床温度上限
    uint16_t adc_hotbed_min_temp; // 热床温度下限
    uint16_t adc_adc_max_temp;    // adc最大采集温度
    uint8_t ui_oled_light;        // oled亮度
    uint8_t fan_auto_flg : 1;     // 自动风扇是否开启
    uint8_t ui_style : 2;         // UI风格,ui_style_t
    uint8_t encoder_rotation : 1; // 编码器旋转方向,enc_rotation_t
    uint8_t pwm_temp_mode;        // 温控模式，0：回流模式，1：恒温模式,temp_mode_t
    bool miot_miot_able;          // 物联网功能使能
    char blinker_id[13];          // 点灯密匙
} user_datas_t;

// uint16_t x = sizeof(user_datas_t);

void user_datas_init();

extern user_datas_t user_datas;
