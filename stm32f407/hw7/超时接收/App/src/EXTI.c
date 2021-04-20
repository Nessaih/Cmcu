#include "EXTI.h"
/********************************************************************************************
*  函 数 名： EXTI_Configration
*  描    述:  PA0外部中断配置
*  输入参数： 无   
*  输出参数： 无
*  返 回 值:  无
*  作    者:  付伟
*  完成日期:  2018-07-21
*  注意事项:  无
********************************************************************************************/
void EXTI_Configration(void)
{
	//IO
	RCC->AHB1ENR |= (0X01|0X01<<4);	//PA,PE时钟开启
	GPIOA->MODER &=~ 0X03;			//PA0输入
	GPIOA->PUPDR &=~ 0X03;			//PA0浮空模式
	GPIOE->MODER &=~ ((0X03<<4)|(0X03<<6)|(0X03<<8));			//PE2,3,4输入
	GPIOE->PUPDR &=~ ((0X03<<4)|(0X03<<6)|(0X03<<8));			//PE2,3,4浮空模式
	
	//EXTI
	RCC->APB2ENR |= 0X01<<14;		//系统配置时钟开启 
	SYSCFG->EXTICR[0] &=~(0X0F|(0X0F<<8)|(0X0F<<12));//外部中断选择：EXTI0选择PA0,
	SYSCFG->EXTICR[0] |=((0X04<<8)|(0X04<<12));//外部中断选择：EXTI2选择PE2,EXTI3选择PE3
	SYSCFG->EXTICR[1] &=~(0X04<<0);		
	SYSCFG->EXTICR[1] |= (0X04<<0);//外部中断选择：EXTI4选择PE4
	
	EXTI->IMR |= (0X01|(0X01<<2)|(0X01<<3)|(0X01<<4));			//EXTI0,2,3,4 中断不屏蔽
	EXTI->EMR &=~ (0X01|(0X01<<2)|(0X01<<3)|(0X01<<4));			//EXTI0,2,3,4 事件屏蔽
	EXTI->RTSR |= (0X01|(0X01<<2)|(0X01<<3)|(0X01<<4));			//上升沿触发
	//EXTI->FTSR |= (0X01|(0X01<<2)|(0X01<<3)|(0X01<<4));			//下降沿触发
	EXTI->SWIER &=~ (0X01|(0X01<<2)|(0X01<<3)|(0X01<<4));		//软件中断不产生
	EXTI->PR |= (0X01|(0X01<<2)|(0X01<<3)|(0X01<<4));			//写1清除中断挂起标志位
	
	//中断分组选择第二组
	NVIC_SetPriorityGrouping(7-2);
	//优先级设置，抢占0，响应0
	NVIC_SetPriority(EXTI0_IRQn,NVIC_EncodePriority(7-2,0,0));
	NVIC_SetPriority(EXTI2_IRQn,NVIC_EncodePriority(7-2,0,0));
	NVIC_SetPriority(EXTI3_IRQn,NVIC_EncodePriority(7-2,0,0));
	NVIC_SetPriority(EXTI4_IRQn,NVIC_EncodePriority(7-2,0,0));
	//中断使能
	NVIC_EnableIRQ(EXTI0_IRQn);									
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);

}

void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & (0X01<<0))
    {
        EXTI->PR |= 0X01<<0;  //清除挂起位
       // printf("EXTI->IRQ\r\n");
        LED1_STATE = !LED1_STATE;
    }
}
void EXTI2_IRQHandler(void)
{
	if(EXTI->PR & (0X01<<2))
    {
        EXTI->PR |= 0X01<<2;  //清除挂起位
       // printf("EXTI->IRQ\r\n");
        LED2_STATE = !LED2_STATE;
    }
}
void EXTI3_IRQHandler(void)
{
	if(EXTI->PR & (0X01<<3))
    {
        EXTI->PR |= 0X01<<3;  //清除挂起位
       // printf("EXTI->IRQ\r\n");
        LED3_STATE = !LED3_STATE;
    }
}
void EXTI4_IRQHandler(void)
{
	if(EXTI->PR & (0X01<<4))
    {
        EXTI->PR |= 0X01<<4;  //清除挂起位
       // printf("EXTI->IRQ\r\n");
        LED4_STATE = !LED4_STATE;
    }
}
