/*
 * Distance Measuring Device
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "wifi.h"
#include "utils.h"

static int quality = 0;
/* WiFi communication buffer */
static char _buffer[150];

/* WiFi connection status */
static uint8_t Connect_Status;


int main(void)
{
    /* enable global interrupts */
    sei();

    /* initialize devices */
    USART_Init(115200);
    esp_connect();
		DDRD |= (1 << PD7);
		DDRD |= (1 << PD6);
		DDRA |= (0 << PA6);
		ADMUX = 0;
		ADMUX |= (0 << MUX0);
		ADMUX |= (1 << REFS0);
		ADCSRA = 0;
		ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
		ADCSRA |= (1 << ADEN);
    while(1) {
				PORTD |= (1 << PD7);
				_delay_ms(280);
				PORTD |= (1 << PD6);
				ADCSRA |= (1 << ADSC);         //start conversion
				loop_until_bit_is_set(ADCSRA, ADIF);  //wait until conversion is done: until ADIF from ADCSRA is set to 1
				// int Vo = ADC;
				// quality = Vo / 1024.0 * 5.0;
				quality = ADC;
				PORTD &= (0 << PD6);
				PORTD &= (0 << PD7);
				esp_send();
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
    sprintf(_buffer, "GET /update?value=%d HTTP/1.0\r\n\r\n", quality);
    ESP8266_Send(_buffer);

}
