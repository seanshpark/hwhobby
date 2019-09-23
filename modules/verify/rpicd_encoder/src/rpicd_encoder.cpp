#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <gpio/gpio_interface.h>
#include <cdgpio/cdgpio_interface.h>

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

int gpio_read(hwboard::CdLine* linea, hwboard::CdLine* lineb)
{
  int rv, value;
  int enc_value = 1000;

  if (!linea->as_input(hwboard::CdLine::InputType::FallingEdge))
    return -1;

  if (!lineb->as_input(hwboard::CdLine::InputType::Normal))
    return -1;

  lcd_show_encoder(enc_value);

  while (!quit)
  {
    struct timespec ts = { 1, 0 };  // 1 sec

    // wait for event
    auto etype = linea->wait_edge(ts);
    if (hwboard::CdLine::EventType::FallingEdge == etype)
    {
      // we want to be sure encoder_b is low. wait 1 msec and read again
      hwboard_delay(1*1000); // 1 msec
      if (!linea->read())
      {
        // linea is low
        // read lineb and detect CW or CCW
        if (!lineb->read())
          enc_value++; // increment for CW
        else
          enc_value--; // decrement for CCW

        lcd_show_encoder(enc_value);
      }
    }
  }

  return 0;
}

int gpiod_test_encoder(void)
{
  hwboard::CdGpio cdgpio;

  if (!cdgpio.open(0))
  {
    std::cout << "cdgpio.open() failed" << std::endl;
    return-1;
  }

  hwboard::CdLine* line17 = cdgpio.line(17);
  hwboard::CdLine* line18 = cdgpio.line(18);

  if (line17 && line18)
  {
    gpio_read(line17, line18);
  }

  cdgpio.close();

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
