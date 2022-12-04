//Functions to initialize and print to OLED display
#include <stdint.h>

bool oled_init();
bool oled_draw_h_line(int x, int y, int len);
bool oled_draw_v_line(int x, int y, int len);
void oled_send_command(uint8_t c);
bool oled_push_to_display(void);
