/*
 * A program to connect exclusively STM32F401RE and Bluetooth module (in my case it was HC05)
 * and then to control two LEDs:
 * 1st: turning it on and off
 * 2nd: controlling the PWM
 */

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>

float pulse = 0;	//PWM = 50%

void RCC_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1 clock enable
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		//GPIOB clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//PWM clock enable
}

void BT_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // PB6 USART1_TX, PB7 USART1_RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Initializing alternate functions
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
}

void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}

void BlinkLED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_BlinkLED;

	GPIO_BlinkLED.GPIO_Pin = GPIO_Pin_8;
	GPIO_BlinkLED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_BlinkLED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_BlinkLED);
}

void PWMLED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_PWMLED;

	GPIO_StructInit(&GPIO_PWMLED);
	GPIO_PWMLED.GPIO_Pin = GPIO_Pin_10;
	GPIO_PWMLED.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_PWMLED.GPIO_Mode = GPIO_Mode_AF;
	GPIO_PWMLED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_PWMLED);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
}

void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_PWMLED;
	TIM_OCInitTypeDef  CHANNEL_PWMLED;

	TIM_TimeBaseStructInit(&TIM_PWMLED);
	TIM_PWMLED.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_PWMLED.TIM_Prescaler = 64 - 1;
	TIM_PWMLED.TIM_Period = 1000 - 1;
	TIM_TimeBaseInit(TIM2, &TIM_PWMLED);

	TIM_OCStructInit(&CHANNEL_PWMLED);
	CHANNEL_PWMLED.TIM_OCMode = TIM_OCMode_PWM1;
	CHANNEL_PWMLED.TIM_OutputState = TIM_OutputState_Enable;
	CHANNEL_PWMLED.TIM_Pulse = pulse;
	TIM_OC3Init(TIM2, &CHANNEL_PWMLED);

	TIM_Cmd(TIM2, ENABLE);
}

void OutString(char *s)
{
  while(*s)
  {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); //Wait for Empty

    USART_SendData(USART1, *s++); //Send char
  }
}

volatile uint32_t timer_ms = 0; 	//Variable for delay function

void SysTick_Handler()
{
	if (timer_ms)
	{
		timer_ms--;
	}
}

void delay_ms(int time)
{
	timer_ms = time;
	while (timer_ms);
}

int main(void)
{

	if (SysTick_Config(SystemCoreClock / 1000))
	{
	//Catch error
	while (1);
	}

	SystemInit();

	RCC_Configuration();

	BT_Configuration();

	USART1_Configuration();

	BlinkLED_Configuration();

	PWMLED_Configuration();

	TIM_Configuration();

	OutString("Welcome to Nucleo F401RE\r\n");

    uint16_t Data;

    while (1)
  	{
    	delay_ms(5);
  		if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
  		{
  		    Data = USART_ReceiveData(USART1);
  		    switch (Data)
  		    {
  						case 'n':	//Stands for ON
  							OutString("\rLED ON\r\n");
  							GPIO_SetBits(GPIOB, GPIO_Pin_8);
  						break;

  						case 'f':	//Stands for OFF
  							OutString("\rLED OFF\r\n");
  							GPIO_ResetBits(GPIOB, GPIO_Pin_8);
  						break;

  						case 'u':	//Stands for UP
  							pulse = pulse + 25;
  							if(pulse > 1000)
  							{
  								pulse = 1000;
  								OutString("\rPWM AT 100%\r\n");
  							}
  							else
							{
								OutString("\rLED PWM 2,5% UP\r\n");
							}
  							TIM_Configuration();
						break;

  						case 'd':	//Stands for DOWN
  							pulse = pulse - 25;
  							if(pulse < 0)
  							{
  								pulse = 0;
  								OutString("\rPWM AT 0%\r\n");
  							}
  							else
  							{
  								OutString("\rLED PWM 2,5% DOWN\r\n");
  							}
  							TIM_Configuration();
						break;

  						default:
  							OutString("\rWrong Command\n");
  							break;
  		    }
  		}
    }
}
