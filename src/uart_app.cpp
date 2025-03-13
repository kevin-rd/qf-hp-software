
#include "User.h"
#include "uart_cmd.h"

extern float kpv, kiv, kdv, kiv_high;

static uint16_t get_num_int(const char *str)
{
    uint16_t num = 0;
    while (*str >= '0' && *str <= '9')
    {
        num = num * 10 + (*str - '0');
        str++;
    }
    return num;
}

static float get_num_f(const char *str)
{
    float tmp = 0;
    uint16_t num = 0;
    uint16_t xiaoshu = 0;
    uint8_t xiaoshu_cnt = 0;
    uint32_t pow = 10;
    while (*str >= '0' && *str <= '9')
    {
        num = num * 10 + (*str - '0');
        str++;
    }
    str++;
    while (*str >= '0' && *str <= '9')
    {
        xiaoshu = xiaoshu * 10 + (*str - '0');
        str++;
        xiaoshu_cnt++;
    }

    if (xiaoshu_cnt > 0)
    {
        for (uint8_t i = 1; i < xiaoshu_cnt; i++)
        {
            pow *= 10;
        }
    }

    tmp = xiaoshu;
    tmp /= pow;
    tmp += num;

    return tmp;
}

void uart_app_handler()
{
    if (uart_cmd_available())
    {
        /// cmd
        if (uart_cmd_cmp("reset", 5))
        {
            Serial.println("reset");
            ESP.restart();
        }
        else if (uart_cmd_cmp("T", 1))
        {
            char buffer[24];
            memset(buffer, 'a', sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = 0;
            uart_cmd_get_string(buffer);
            user_datas.pwm_temp_buf = get_num_int((const char *)&buffer[1]);
            Serial.printf("%d\n", user_datas.pwm_temp_buf);
        }
        else if (uart_cmd_cmp("P", 1))
        {
            char buffer[24];
            memset(buffer, 'a', sizeof(buffer) - 1);
            buffer[sizeof(buffer) - 1] = 0;
            uart_cmd_get_string(buffer);
            kpv = get_num_f((const char *)&buffer[1]);
            Serial.printf("SET KP:%f\n", kpv);
        }
        else if (uart_cmd_cmp("IH", 2))
        {
            char buffer[24];
            uart_cmd_get_string(buffer);
            kiv_high = get_num_f((const char *)&buffer[2]);
            Serial.printf("SET KIH:%f\n", kiv_high);
        }
        else if (uart_cmd_cmp("I", 1))
        {
            char buffer[24];
            uart_cmd_get_string(buffer);
            kiv = get_num_f((const char *)&buffer[1]);
            Serial.printf("SET KI:%f\n", kiv);
        }
        else if (uart_cmd_cmp("D", 1))
        {
            char buffer[24];
            uart_cmd_get_string(buffer);
            kdv = get_num_f((const char *)&buffer[1]);
            Serial.printf("SET KD:%f\n", kdv);
        }
        else if (uart_cmd_cmp("get_pid", 7))
        {
            Serial.printf("KP:%f\n", kpv);
            Serial.printf("KI:%f\n", kiv);
            Serial.printf("KI HIGH:%f\n", kiv_high);
            Serial.printf("KD:%f\n", kdv);
        }
        else
        {
            char buffer[24];
            uart_cmd_get_string(buffer);
            Serial.printf("input cmd error, cmd:%s\n", buffer);
        }

        uart_cmd_clear();
    }
}
