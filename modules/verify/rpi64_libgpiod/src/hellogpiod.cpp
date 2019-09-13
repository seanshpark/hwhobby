#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <gpiod.h>

#include <gpio/gpio_interface.h>
#include <hwmodule/lcd1602.h>

volatile sig_atomic_t quit = 0;

void signal_handler(int sig)
{
  signal(sig, signal_handler);
  quit = 1;
}

#define I2C_ADDR_BLUE   0x3F            // my blue backlight LCD is 0x3f
#define I2C_ADDR_YELLOW 0x27            // my yellow backlight LCD is 0x27
#define I2C_ADDR        I2C_ADDR_BLUE

HWRESULT lcd_init(void)
{
  HW_I2C_INIT_t i2cinit;

  i2cinit.i2caddr = I2C_ADDR;
  if (lcd1602_init(&i2cinit) != HWRESULT_SUCCESS)
    return HWRESULT_FAILED;

  lcd1602_home();
  lcd1602_clear();

  return HWRESULT_SUCCESS;
}

void lcd_show_encoder(int value)
{
  char buffer[32];

  sprintf(buffer, "Encoder %4d", value);
  lcd1602_moverc(0, 1);
  lcd1602_puts(buffer);

  printf("LCD: %s\r\n", buffer);
}

int gpio_read(struct gpiod_line* linea, struct gpiod_line* lineb)
{
  int rv, value;
  int enc_value = 1000;

  rv = gpiod_line_request_falling_edge_events(linea, "encoder_a");
  if (rv != 0)
    return -1;

  rv = gpiod_line_request_input(lineb, "encoder_b");
  if (rv != 0)
    return -1;

  lcd_show_encoder(enc_value);

  while (!quit)
  {
    struct timespec ts = { 1, 0 };  // 1 sec

    // wait for event
    rv = gpiod_line_event_wait(linea, &ts);
    if (rv == 1)
    {
      // event fired
      struct gpiod_line_event ev;
      rv = gpiod_line_event_read(linea, &ev);
      if (rv == 0 && ev.event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
      {
        // event was read correctly and it's falling edge
        // we want to be sure encoder_b is low. wait 1 msec and read again
        hwboard_delay(1*1000); // 1 msec
        value = gpiod_line_get_value(linea);
        if (value == 0)
        {
          // linea is low, now read lineb and detect CW or CCW
          value = gpiod_line_get_value(lineb);
          if (value == 0)
            enc_value++; // increment for CW
          else
            enc_value--; // decrement for CCW

          lcd_show_encoder(enc_value);
        }
      }
    }
  }

  return 0;
}

int gpiod_test_encoder(void)
{
  struct gpiod_chip* chip;
  struct gpiod_line* line17;
  struct gpiod_line* line18;

  chip = gpiod_chip_open("/dev/gpiochip0");
  if (!chip)
  {
    std::cout << "gpiod_chip_open failed" << std::endl;
    return-1;
  }

  // connect encoder A to GPIO17 and B to GPIO18
  line17 = gpiod_chip_get_line(chip, 17);
  line18 = gpiod_chip_get_line(chip, 18);
  if (line17 && line18)
  {
    gpio_read(line17, line18);
  }

  if (line17)
    gpiod_line_release(line17);
  if (line18)
    gpiod_line_release(line18);

  gpiod_chip_close(chip);

  return 0;
}

int main()
{
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  if (hwboard_gpio_init() != HWRESULT_SUCCESS)
    return -1;

  if (lcd_init() != HWRESULT_SUCCESS)
    return -1;

  hwboard_delay(10*1000); // 10 msec

  return gpiod_test_encoder();
}
