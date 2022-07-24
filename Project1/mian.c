#include "stm32f4xx.h"

#define SET1(x) (1ul << x)
#define SET0(x) (~SET1(x))

void usart2_init(void);
void sendData(char data);
void analog1_init(void);
char getChar(int number);
void init(void);
uint16_t getTemprature(void);
void writeNumber(uint16_t number);
void delay(volatile int number);

void init(void){
	usart2_init();
	analog1_init();
}

void usart2_init(void){
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // activing GPIOA clock
	RCC -> APB1ENR |= RCC_APB1ENR_USART2EN; // activing USART2
	GPIOA -> MODER |= GPIO_MODER_MODER2_1; // setting pin 2 as alternate function
	
	GPIOA -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_1;
	GPIOA -> AFR[0]|= 0x00000700;
	
	USART2 -> CR1 |= USART_CR1_UE;
	USART2 -> BRR = SystemCoreClock / 115200;
	USART2 -> CR1 |= USART_CR1_TE;
}

void sendData(char data){
	USART2 -> DR = ((uint8_t) data);
	while(!(READ_BIT(USART2 -> SR, USART_SR_TC)))
		;
}

char getChar(int number){
	switch(number){
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
	}//never gets here!
	return ' ';
}

void analog1_init(void){
	RCC -> AHB1ENR |= 1; // enabling GPIOA clock
	GPIOA -> MODER |= 0xC; // mode register = analoge
	
	RCC -> APB2ENR |= 0x00000100; // enabling ADC1 clock
	ADC1 -> CR2 = 0; // sw trigger
	ADC1 -> SQR3 = 1; // conversion start at channel 1
	ADC1 -> SQR1 = 0; // conversion length = 1
	ADC1 -> CR2 |= 1; // enabling ADC1
}

uint16_t getTemprature(void){
	uint16_t temprature;
	while(1){
		ADC1->CR2 |= 0x40000000;  // starting the conversion
		while( !(ADC1->SR & 2)) // waiting for conversion to be finished
			;
		temprature = ADC1 -> DR; // getting the value between 0 and 4095
		break;
	}
	double res = temprature;
	res /= 4095;
	res *= 3.3;
	res *= 100;
	temprature = (uint16_t) res;
	return temprature;
}

void writeNumber(uint16_t number){
	if(number == 0){
		sendData('0');
		return;
	}else{
		if(number / 10 > 0)
			writeNumber(number / 10);
		sendData(getChar(number % 10));
	}
}

void delay(volatile int number){
	for(int i = 0; i < number; i++)
		for(int j = 0; j < number; j++)
			;
}
int main(void){
	init();
	sendData('S');sendData('t');sendData('a');sendData('r');sendData('t');sendData('\n\r');
	delay(2000); // waiting for proteus (specially lm35) to get ready and giving the correct voltage
	while(1){
		uint16_t temperature = getTemprature();
		writeNumber(temperature);
		sendData('\n\r'); // new Line
		delay(2000);
	}
}
