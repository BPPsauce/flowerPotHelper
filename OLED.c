//Implementation of OLDED.h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include "utils.h"

#define SSD1306_MEMORYMODE 0x20          ///< 
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V
#define SSD1306_DEACTIVATESCROLL 0x2E

#define I2C_BUS_1 "/dev/i2c-1"
#define DISPLAY_SLAVE_ADDR 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

static int i2c_file_desc;
static uint8_t *buffer;

static void i2cpinsInit(){
    //init for P_18 and P_17
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
}

static bool is_in_bounds(int x, int y)
{
    if(x < 0 || x > SCREEN_WIDTH || y < 0 || y > SCREEN_HEIGHT){
        return false;
    }
    
    return true;
    
}

//init for the i2c bus
static void initI2cBus (char* bus, int address){
    i2cpinsInit();
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
    }
    i2c_file_desc = i2cFileDesc;
}

//taken from the tutorials
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value){
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);

    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

static bool oled_draw_pixel(int x, int y)
{
    //add bits to buffer to represent a pixel
    if(!is_in_bounds(x, y))
    {
        return false;
    }
    buffer[x + (y / 8) * SCREEN_WIDTH] |= (1 << (y & 7)); // placement in buffer determines where the pixel goes
    return true;
}

void oled_draw_h_line(int x, int y, int len)
{

}
void oled_send_command(uint8_t c)
{
    //send control byte 0x00 then our command
    initI2cBus(I2C_BUS_1, DISPLAY_SLAVE_ADDR);
    writeI2cReg(i2c_file_desc, 0x0, 0x0);
    writeI2cReg(i2c_file_desc, c, 0x0); // write to address, value does not matter 
}

bool oled_init()
{
    buffer = (uint8_t*)malloc(SCREEN_WIDTH * (SCREEN_HEIGHT*7)/8);
    if(!buffer){
        return false;
    }
    
    //set up procedure mimicing adafruit library procedure
    oled_send_command(SSD1306_DISPLAYOFF);

    oled_send_command(SSD1306_SETDISPLAYOFFSET);
    oled_send_command(0x0);

    oled_send_command(SSD1306_SETSTARTLINE);
    oled_send_command(SSD1306_CHARGEPUMP);
    
    oled_send_command(SSD1306_MEMORYMODE);
    oled_send_command(0x0);
    
    oled_send_command(SSD1306_COMSCANDEC);
    
    oled_send_command(SSD1306_SETVCOMDETECT);
    oled_send_command(0x40);
    
    oled_send_command(SSD1306_DISPLAYALLON_RESUME);
    oled_send_command(SSD1306_NORMALDISPLAY);
    oled_send_command(SSD1306_DEACTIVATESCROLL);
    oled_send_command(SSD1306_DISPLAYON);
    return true;
}
void oled_push_to_display(void)
{
    //send control byte 0x40 then our data content in buffer

}

int main(void)
{
    //test main
    
}