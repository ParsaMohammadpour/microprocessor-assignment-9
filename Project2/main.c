/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// LCD pins number in port A
#define data_port GPIOA
#define d0 GPIO_PIN_0 //PA0
#define d1 GPIO_PIN_1 //PA1
#define d2 GPIO_PIN_2 //PA2
#define d3 GPIO_PIN_3 //PA3
#define d4 GPIO_PIN_4 //PA4
#define d5 GPIO_PIN_5 //PA5
#define d6 GPIO_PIN_6 //PA6
#define d7 GPIO_PIN_7 //PA07

#define command_port GPIOC
#define E GPIO_PIN_2  //PB2
#define RW GPIO_PIN_1 //PB1
#define RS GPIO_PIN_0 //PB0

// Keypad pins number in port B
#define keypad_port GPIOB
#define A GPIO_PIN_4//PC0
#define B GPIO_PIN_5//PC1
#define C GPIO_PIN_6//PC2
#define D GPIO_PIN_7//PC3

#define one   GPIO_PIN_0 //PC4
#define two   GPIO_PIN_1 //PC5
#define three GPIO_PIN_2 //PC6
#define four  GPIO_PIN_3 //PC7

//MASKS:
#define SET1(x) (1ul << (x))
#define SET0(x) (~(SET1(x)))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static volatile char array[16];
static volatile int size = 0; // return the first empty index
static volatile int status = 0; // 0 = usual      1 = finding result       2 = clear
static volatile int mul = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void initialize(void);
void keypad_init(void);
char findKeypadButton(void);
void LCD_put_char(char data);
void LCD_init(void);
void LCD_command(unsigned char command);
void LCD_resetCommand(void);
void LCD_setCommand(void);
int getNumber(char c);
char getChar(int digit);
void calculate(void);
void resetArray(void);
void delay(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void initialize(void){
	//setting keypad input to 1
	keypad_init();
	
	//writing names to LCD
	LCD_init();
	delay();
	LCD_setCommand();
	
	// setting all array indexes to "&". (we consider this as a null index in array)
	resetArray();
}
void keypad_init(void){
	HAL_GPIO_WritePin(keypad_port, A, 1);
	HAL_GPIO_WritePin(keypad_port, B, 1);
	HAL_GPIO_WritePin(keypad_port, C, 1);
	HAL_GPIO_WritePin(keypad_port, D, 1);
}

char findKeypadButton(void){
	if(HAL_GPIO_ReadPin(keypad_port, one) == 1){
		char c = ' ';
		HAL_GPIO_WritePin(keypad_port, A, 0);
		if(HAL_GPIO_ReadPin(keypad_port, one) == 0)
			c = '7';
		HAL_GPIO_WritePin(keypad_port, B, 0);
		if(HAL_GPIO_ReadPin(keypad_port, one) == 0)
			c = '4';
		HAL_GPIO_WritePin(keypad_port, C, 0);
		if(HAL_GPIO_ReadPin(keypad_port, one) == 0)
			c = '1';
		HAL_GPIO_WritePin(keypad_port, D, 0);
		if(HAL_GPIO_ReadPin(keypad_port, one) == 0){
			c = ' ';
			status = 2;
		}
		keypad_init();
		return c;
	}else if (HAL_GPIO_ReadPin(keypad_port, two) == 1){
		char c = ' ';
		HAL_GPIO_WritePin(keypad_port, A, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '8';
		HAL_GPIO_WritePin(keypad_port, B, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '5';
		HAL_GPIO_WritePin(keypad_port, C, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '2';
		HAL_GPIO_WritePin(keypad_port, D, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '0';
		keypad_init();
		return c;
	}else if (HAL_GPIO_ReadPin(keypad_port, three) == 1){
		char c = ' ';
		HAL_GPIO_WritePin(keypad_port, A, 0);
		if(HAL_GPIO_ReadPin(keypad_port, three) == 0)
			c = '9';
		HAL_GPIO_WritePin(keypad_port, B, 0);
		if(HAL_GPIO_ReadPin(keypad_port, three) == 0)
			c = '6';
		HAL_GPIO_WritePin(keypad_port, C, 0);
		if(HAL_GPIO_ReadPin(keypad_port, three) == 0)
			c = '3';
		HAL_GPIO_WritePin(keypad_port, D, 0);
		if(HAL_GPIO_ReadPin(keypad_port, three) == 0){
			c = ' ';
			status = 1;
		}
		keypad_init();
		return c;
	}else if (HAL_GPIO_ReadPin(keypad_port, four) == 1){
		char c = ' ';
		HAL_GPIO_WritePin(keypad_port, A, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '/';
		HAL_GPIO_WritePin(keypad_port, B, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '*';
		HAL_GPIO_WritePin(keypad_port, C, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '-';
		HAL_GPIO_WritePin(keypad_port, D, 0);
		if(HAL_GPIO_ReadPin(keypad_port, two) == 0)
			c = '+';
		keypad_init();
		return c;
	}//never gets here
	return ' ';
}




void LCD_put_char(char data) {
	GPIOA -> ODR = data;
	HAL_GPIO_WritePin(command_port, RS, 1);//GPIOA -> ODR |= SET1(RS);
	HAL_GPIO_WritePin(command_port, RW, 0);// GPIOA -> ODR &= SET0(RW);
  HAL_GPIO_WritePin(command_port, E, 1); // GPIOA -> ODR |= SET1(E);
	HAL_GPIO_WritePin(command_port, E, 0); // GPIOA -> ODR &= SET0(E);
	delay();
}

void LCD_init(void){
	LCD_command(0x38);
	delay();
	LCD_command(0x06);
	delay();
	LCD_command(0x08);
	delay();
	LCD_command(0x0F);
	delay();
}

void LCD_command(unsigned char command){
	GPIOA -> ODR = command;
	HAL_GPIO_WritePin(command_port, RS, 0);//GPIOA -> ODR &= SET0(RS);
	HAL_GPIO_WritePin(command_port, RW, 0);//GPIOA -> ODR &= SET0(RW);
  HAL_GPIO_WritePin(command_port, E, 1);//GPIOA -> ODR |= SET1(E);
  delay();
  HAL_GPIO_WritePin(command_port, E, 0);//GPIOA -> ODR &= SET0(E);

  if (command < 4)
      delay();           /* command 1 and 2 needs up to 1.64ms */
  else
      delay();           /* all others 40 us */
}
void delay(void) {
	for(int i = 0; i < 15; i++)
		HAL_Delay(0);
}
int getNumber(char c){
	switch(c){
		case '0' : return 0;
		case '1' : return 1;
		case '2' : return 2;
		case '3' : return 3;
		case '4' : return 4;
		case '5' : return 5;
		case '6' : return 6;
		case '7' : return 7;
		case '8' : return 8;
		case '9' : return 9;
	}
	return -1;
}
void LCD_setCommand(void){
	LCD_command(0x01);
	delay();
	LCD_command(0x38);
	delay();
}

void LCD_resetCommand(void){
	LCD_command(0x01);
	delay();
	LCD_command(0x80);
	delay();
	for(int i = 0; i < 16; i++){
		LCD_put_char(' ');
		delay();
	}	
	LCD_command(0x80);
	delay();
	int i = 0;
	while(i < size){
		LCD_put_char(array[i++]);
		delay();
	}
}


char getChar(int digit){
	switch(digit){
		case 0 : return '0';
		case 1 : return '1';
		case 2 : return '2';
		case 3 : return '3';
		case 4 : return '4';
		case 5 : return '5';
		case 6 : return '6';
		case 7 : return '7';
		case 8 : return '8';
		case 9 : return '9';
	}
	return '&';
}
void resetArray(void){
	size = 0;
	for(int i = 0; i< 16; i++){
		array[i] = ' ';
		delay();
	}
}
void calculate(void){
	int op1 = 0, op2 = 0, total;
	int counter = 0;
	if(getNumber(array[counter]) == -1){
		counter = 1;
		delay(); // this is necessary, otherwise it won't work properly.(it seems, it doesn't have enough time to be executed)
	}
	while((counter < size) && (getNumber(array[counter]) >= 0)){
		op1 *= 10;
		op1 += getNumber(array[counter++]);
	}
	int operatorIndex = counter;
	if(getNumber(array[0]) == -1){
		op1 = -op1;
	}
	total = op1;
	if(counter == size || array[counter] == ' '){
		return;
	}
	counter++;
	while(counter < size){
		op2 *= 10;
		op2 += getNumber(array[counter++]);
	}
	switch(array[operatorIndex]){
		case '+' : total = op1 + op2; break;
		case '-' : total = op1 - op2; break;
		case '*' : total = op1 * op2; break;
		case '/' : total = op1 / op2; break;
	}
	resetArray();
	if(total == 0){
		array[size++]='0';
		return;
	}
	char digit[8];
	if(total < 0){
		array[size++] = '-';
		total = -total;
	}
	
	mul = 3 * total;		
	
	
	int index = 0;
	while(total > 0){
		digit[index++] = getChar(total % 10);
		total /= 10;
	}
	while(index > 0)
		array[size++] = digit[--index];
	
	
	
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	LCD_put_char('i');
	if(GPIO_Pin & SET1(0)){
		LCD_put_char('n');
		char res = findKeypadButton();
		HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn); // to avoid making an infinite loop due to making changes that lead to interrupt in keypad
		LCD_put_char(res);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	initialize();
	LCD_put_char('s');
	//GPIOB -> ODR |= SET1(4) | SET1(5) | SET1(6) | SET1(7);
	HAL_GPIO_WritePin(keypad_port, A, 1);
	HAL_GPIO_WritePin(keypad_port, B, 1);
	HAL_GPIO_WritePin(keypad_port, C, 1);
	HAL_GPIO_WritePin(keypad_port, D, 1);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
