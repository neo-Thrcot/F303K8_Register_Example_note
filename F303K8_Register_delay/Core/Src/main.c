#include "stm32f3xx.h"

void System_Clock_Init(void);

void delay_ms(uint32_t ms);

int main(void)
{
	System_Clock_Init();

	RCC -> AHBENR |= (1 << 17);

	GPIOA -> MODER |= (1 << 0);
	GPIOA -> MODER &= (~(1 << 1));

	while (1)
	{
		GPIOA -> ODR ^= (1 << 0);
		delay_ms(1000);
	}

	return 0;
}

void System_Clock_Init(void)
{
	/*PLLの設定*/
	RCC -> CFGR &= (~(1 << 16));		//PLLのクロック源をHSIに
	RCC -> CFGR |= (4 << 8);			//APB1のクロックを2分周
	RCC -> CFGR |= (15 << 18);			//PLLのクロックを16倍に逓倍

	/*Flash読み出しの遅延*/
	FLASH -> ACR |= (2 << 0);

	/*PLLを有効にし、有効になるまで待機*/
	RCC -> CR |= (1 << 24);
	while (!(RCC -> CR & (1 << 25)));

	/*システムクロックをPLLに変更し、有効になるまで待機*/
	RCC -> CFGR |= (2 << 0);
	while ((RCC -> CFGR & (3 << 2)) != (2 << 2));
}

void delay_ms(uint32_t ms)
{
	/*SysTickの設定*/
	SysTick -> LOAD = 8000 - 1;			//リセット値をセット
	SysTick -> VAL = 0;					//カウントを0にする
	SysTick -> CTRL |= (1 << 0);		//SysTickを有効にする

	for (int i = 0; i < ms; i++) {
		/*1ms待機をms回繰り返す*/
		while (!(SysTick -> CTRL & (1 << 16)));
	}

	SysTick -> CTRL &= (~(1 << 0));		//SysTickを無効にする
}
