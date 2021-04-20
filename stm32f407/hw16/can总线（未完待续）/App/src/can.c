#include "can.h"


void Can_Configuration(void)
{
	uint8 delay = 50;
	//double NominalBitTime,TQ;
	uint16 BRP90;
	//CAN2总线RX，TX复用开启
	RCC->AHB1ENR = 0X01<<1;//PB时钟开启
	GPIOB->MODER &=~ ((0X10<<12)|(0X10<<13));
	GPIOB->MODER |= (0X10<<12)|(0X10<<13);
	GPIOB->OSPEEDR &=~ ((0X10<<12)|(0X10<<13));
	GPIOB->OSPEEDR |= 0X01<<13;//25Mhz输出速度
	GPIOB->PUPDR &=~ ((0X10<<12)|(0X10<<13));
	GPIOB->PUPDR &=~ ((0X01<<12)|(0X01<<13));//端口上拉
	
	//CAN相关寄存器配置
	RCC->APB1ENR = 0X01<<26;//can2时钟开启
	CAN2->MCR |= (0X01<<6)|(0X01<<3)|(0X01);	/*硬件自动检测128个11个隐性位，退出总线关闭状态;
												FIFO锁定，优先接收旧消息;
												CAN进入初始化模式，允许修改BTR寄存器.*/
	
	do{
		delay_ms(1);
	}
	while(!(CAN2->MSR&0X01)&&delay);//
	
//	NominalBitTime = 1/BaudRate;
//	TQ = NominalBitTime/TQ_NUM;
//	BRP90 = TQ*42000000-1;
	BRP90 = 42000/(BaudRate*TQ_NUM)-1;
	CAN2->TSR |= (BRP90&0X1FF);
	CAN2->TSR |=(0X07<<16)|(0X06<<20);//1,8,7
	CAN2->MCR &=~ 0X01;//退出初始化模式
	
	CAN2->FMR |= 0X01;//筛选器初始化
	CAN2->FM1R |= 0XFFFFFFF;//列表模式
	CAN2->FFA1R &=~ 0X01;//筛选器0分配到FIFO 0;
	CAN2->FA1R |= 0X01;//激活筛选器0
	CAN2->FMR &=~ 0X01;//筛选器退出初始化
}
