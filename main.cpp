#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "sreglib/ShiftRegister74HC595.hpp"

//spi interface, either spi0 or spi1, 
//make sure to pick the right pins according to the diagram provided in the pico datasheet
#define SPI_PORT spi0 

#define SCK_PIN 2 //clock pin
#define SDI_PIN 3 //data pin
#define LATCH_PIN 4 //latch pin
#define DISPLAY_SIZE 3 //number of digits in display

// create a global shift register object
// parameters: <number of shift registers> (spi port, clock pin, data pin, latch pin)
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

//decimal point, use bitwise or with other numbers to enable
uint8_t decimal_point = 0b10000000; 

//array with length the same as the number of digits in your display
uint8_t test_array[DISPLAY_SIZE] = { 
    //put any numbers you want, right to left, use "| decimal_point" to add a dot to it.
    // (of course you can put any symbol you want with the correct bit sequence, consult your display datasheet for more information).
    disp_digits[0],
    disp_digits[2], // | decimal_point, //use "| decimal_point to add a dot to any digit"
    disp_digits[4]
};


// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "legos";
const char WIFI_PASSWORD[] = "deborahmaia";

int main() {
    sr.setAllLow(); //set all pins to off
    sleep_ms(1000);

    sr.set(0, 1); //set a specific pin to on, 

    stdio_init_all();

    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    uint8_t val = 1;
    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
        sr.set(1, val); //set a specific pin to on,
        val ? val = 0 : val = 1;
        sleep_ms(1000);
    }
    // Print a success message once connected
    printf("Connected! \n");
    sr.set(1, 1); //set a specific pin to on, 
    
    cyw43_t cyw43_state;
    uint8_t mac[6];
    cyw43_wifi_get_mac (&cyw43_state, CYW43_ITF_AP, mac);
    
    printf ("IP address: ");
    for( int i = 0; i < 6; i++ )
    {
        printf("%" PRIu8 ":", mac[i]);
    }
    printf ("\n");

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");
    sr.set(2, 1); //set a specific pin to on, 
    sleep_ms(1000);

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    sr.set(3, 1); //set a specific pin to on, 
    sleep_ms(1000);

    cgi_init();
    printf("CGI Handler initialised\n");
    sr.set(4, 1); //set a specific pin to on, 
    sleep_ms(1000);


    // Infinite loop
    while(1)
    {
        // sr.setAll(test_array); //set diplay to the contents of the test array
        // sleep_ms(1000);
        // sr.setAllLow(); //set all pins to off
        // sleep_ms(1000);
        // sr.setAllHigh(); //set all pins to on
        // sleep_ms(1000);
        // sr.setAllLow(); //set all pins to off
        // sleep_ms(1000);
        // sr.set(12, 1); //set a specific pin to on, 
        // sleep_ms(1000);
        // sr.set(12, 0); //set a specific pin to off,
        // sleep_ms(1000);
    }
}