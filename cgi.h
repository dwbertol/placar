#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "sreglib/ShiftRegister74HC595.hpp"
#include <map>

//spi interface, either spi0 or spi1, 
//make sure to pick the right pins according to the diagram provided in the pico datasheet
#define SPI_PORT spi0 

#define SCK_PIN 2 //clock pin
#define SDI_PIN 3 //data pin
#define LATCH_PIN 4 //latch pin
#define DISPLAY_SIZE 11 //number of digits in display

ShiftRegister74HC595<DISPLAY_SIZE> sr(SPI_PORT, SCK_PIN, SDI_PIN, LATCH_PIN);

uint8_t disp_digits[10] = { //decimal numbers, each array index corresponds to the right number
	0b00111111, //0
	0b00000110, //1
	0b01011011, //2
	0b01001111, //3
	0b01100110, //4
	0b01101101, //5
	0b01111101, //6
	0b00000111, //7
	0b01111111, //8
	0b01101111 //9
};

std::map<uint8_t, uint> dtoi{{63, 0}, {6, 1}, {91, 2}, {79, 3}, {102, 4}, {109, 5}, {125, 6}, {7, 7}, {127, 8}, {111, 9}};
std::map<uint, uint8_t> itod{{0, 63}, {1, 6}, {2, 91}, {3, 79}, {4, 102}, {5, 109}, {6, 125}, {7, 7}, {8, 127}, {9, 111}};

uint digitToInt(uint8_t asciiDigit) {
    if (asciiDigit >= 256)
        return 0;
    
    return dtoi.find(asciiDigit)->second;
}

uint8_t intToDigit(uint integer) {
    uint val = integer%10;
    
    return itod.find(val)->second;
}

//array with length the same as the number of digits in your display
uint8_t test_array[DISPLAY_SIZE] = { 
    //put any numbers you want, right to left, use "| decimal_point" to add a dot to it.
    // (of course you can put any symbol you want with the correct bit sequence, consult your display datasheet for more information).
    disp_digits[0],
    disp_digits[0], // | decimal_point, //use "| decimal_point to add a dot to any digit"
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
    disp_digits[0],
};


// CGI handler which is run when a request for /led.cgi is detected
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0] , "led") == 0){
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if(strcmp(pcValue[0], "1") == 0){
            if(cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN))
            {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                sr.set(12, 0); //set a specific pin to off, 
            }
            else
            {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                sr.set(12, 1); //set a specific pin to on, 
            }
        }
    }
    
    // Send the index page back to the user
    return "/setup.html";
}

const char * cgi_local_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    sr.set(0, 1);
    // Check if an request for LED has been made (/led.cgi?led=x)
    if (strcmp(pcParam[0] , "plus") == 0){
        // Look at the argument to check if LED is to be turned on (x=1) or off (x=0)
        if(strcmp(pcValue[0], "1") == 0){
            // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            test_array[2] = intToDigit(digitToInt(test_array[2]) + 1);
            sr.setAll(test_array);
        }
    }
    
    // Send the index page back to the user
    return "/volei.html";
}

// tCGI Struct
// Fill this with all of the CGI requests and their respective handlers
static const tCGI cgi_handlers[] = {
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/led.cgi", cgi_led_handler
    },
    {
        // Html request for "/led.cgi" triggers cgi_handler
        "/local.cgi", cgi_local_handler
    },
};

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 2);
}