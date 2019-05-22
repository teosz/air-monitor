/*
 * Distance Measuring Device
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
#include "wifi.h"
#include "utils.h"

/* time elapsed after the previous server communication */
static int wifi_send_timer;

/* WiFi communication buffer */
static char _buffer[150];

/* WiFi connection status */
static uint8_t Connect_Status;

int main(void)
{
    /* initialize devices */
    USART_Init(115200)
    esp_connect();

    /* main loop */
    while(1) {
				esp_send();
        _delay_ms(500);
    }
}


/* connect to ESP module and open TCP connection with the server */
void esp_connect()
{
    while(!ESP8266_Begin());
    ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);/* 3 = Both (AP and STA) */
    ESP8266_ConnectionMode(SINGLE);		/* 0 = Single; 1 = Multi */
    ESP8266_ApplicationMode(NORMAL);	/* 0 = Normal Mode; 1 = Transperant Mode */

    if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP) {
        ESP8266_JoinAccessPoint(SSID, PASSWORD);
    }

    ESP8266_Start(0, DOMAIN, PORT);
}

/* send value to server */
void esp_send()
{
    Connect_Status = ESP8266_connected();
    if(Connect_Status == ESP8266_NOT_CONNECTED_TO_AP)
    ESP8266_JoinAccessPoint(SSID, PASSWORD);
    if(Connect_Status == ESP8266_TRANSMISSION_DISCONNECTED)
    ESP8266_Start(0, DOMAIN, PORT);

    memset(_buffer, 0, 150);
    sprintf(_buffer, "GET /update?api_key=%s&field1=%d", API_WRITE_KEY, min_distance);
    ESP8266_Send(_buffer);

}
