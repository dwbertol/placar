#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"



//decimal point, use bitwise or with other numbers to enable
uint8_t decimal_point = 0b10000000; 

// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "legos";
const char WIFI_PASSWORD[] = "legosmobi";

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
    
    sr.setAllLow(); //set all pins to off
    sleep_ms(2000);
    sr.setAllHigh(); //set all pins to on
    sleep_ms(2000);
    sr.setAllLow(); //set all pins to off
    sleep_ms(2000);
    sr.setAll(test_array); //set diplay to the contents of the test array

    // Infinite loop
    while(1)
    {
        
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