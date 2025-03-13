#include "User.h"

#include "EEPROM.h"

#include "uart_app.h"

/*
  启凡科创QF-HP物联网加热台固件源码
  版本:V1.64
  日期:2024-4-23
*/

void setup()
{
  user_datas_init();
  delay(100);

#ifdef DEBUG
  {
    Serial.begin(115200);
  }

#endif

  oled.begin();

  eeprom.read_all_data();

  Ticker_init();

  miot.begin();

  if (user_datas.encoder_rotation == ENC_ROTATION_CCW)
    ec11.begin(5, 2, 4, ui_key_callb, ec11_ccw);
  else
    ec11.begin(5, 4, 2, ui_key_callb, ec11_cw);

  ec11.speed_up(true);
  ec11.speed_up_max(20);

  ui.page_switch_flg = true;
}

void loop()
{
  static uint16_t cnt = 0;
  cnt++;
  if (cnt == 0)
    delay(1);
  if (system_get_cpu_freq() != SYS_CPU_160MHZ)
    system_update_cpu_freq(SYS_CPU_160MHZ);
  ui.run_task();
  eeprom.write_task();
  miot.run_task();
  while (Serial.available())
    uart_cmd_get_byte(Serial.read());
  uart_app_handler();
}
