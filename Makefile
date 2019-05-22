main:
	avr-gcc -mmcu=atmega324p -Os -Wall -o main.elf main.c wifi.c USART_RS232.c lcd.c
	avr-objcopy  -j .text -j .data -O ihex main.elf main.hex

upload:
	sudo ~/test/bootloadHID.2014-03-29/commandline/bootloadHID -r main.hex

clean:
	rm -rf *.elf *.hex
