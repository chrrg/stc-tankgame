#include "includes.h"

sbit LED1 = P5^5;//运行灯
sbit LED2 = P4^2;
//sbit KEY  = P3^3;//自带按键

sbit LCD_PSB =  P4^6;      //串/并方式控制
sbit LCD_RST =  P4^7;      //液晶复位端口

//定义任务堆栈

OS_STK Task1Stk[MaxStkSize+1];
OS_STK Task2Stk[MaxStkSize+1];
OS_STK Task3Stk[MaxStkSize+1];
OS_STK TaskFeedStk[MaxStkSize+1];

void Task_Feed(void *ppdata) reentrant{ET0=1;for(;;){WDT_CONTR = 0x3f;H_DOG = ~H_DOG;OSTimeDly(OS_TICKS_PER_SEC*3);}}//喂狗专属
void Task1(void *ppdata) reentrant{//监听输入
	//ppdata = ppdata;
	ET0=1;
	for(;;){
		TankGame_Input(OSTimeGet());
		OSTimeDly(10);
	}
}

void Task2(void *ppdata) reentrant{//游戏渲染程序
	//ppdata=ppdata;
	for(;;){
		TankGame_Render();
		OSTimeDly(2);
  }
}
void Task3(void *ppdata) reentrant{//测试
	//ppdata=ppdata;
	for(;;){
		LED1=~LED1;//红灯 0亮1灭
		OSTimeDly(100);
	}
}
//Game game;

void main(void){
	OSInit();
	//初始化定时器
	TMOD &= 0xF0;TMOD |= 0x01;TR0 = 1;
	//芯片IO口初始化
	P0M0 = 0X00;P0M1 = 0X00;P1M0 = 0X00;P1M1 = 0X00;P2M0 = 0X00;P2M1 = 0X00;P3M0 = 0X00;P3M1 = 0X00;P4M0 = 0X00;P4M1 = 0X00;
	
	TankGame_Init();//游戏初始化
	OSTaskCreate(Task1,(void*)0,&Task1Stk[0],0);//交互
	OSTaskCreate(Task2,(void*)0,&Task2Stk[0],1);//渲染
	OSTaskCreate(Task3,(void*)0,&Task3Stk[0],2);//测试使用
	OSTaskCreate(Task_Feed,(void*)0,&TaskFeedStk[0],3);//喂狗 优先级数字越小越优先
	OSStart();
}