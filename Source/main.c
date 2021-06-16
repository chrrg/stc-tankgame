#include "includes.h"

sbit LED1 = P5^5;//���е�
sbit LED2 = P4^2;
//sbit KEY  = P3^3;//�Դ�����

sbit LCD_PSB =  P4^6;      //��/����ʽ����
sbit LCD_RST =  P4^7;      //Һ����λ�˿�

//���������ջ

OS_STK Task1Stk[MaxStkSize+1];
OS_STK Task2Stk[MaxStkSize+1];
OS_STK Task3Stk[MaxStkSize+1];
OS_STK TaskFeedStk[MaxStkSize+1];

void Task_Feed(void *ppdata) reentrant{ET0=1;for(;;){WDT_CONTR = 0x3f;H_DOG = ~H_DOG;OSTimeDly(OS_TICKS_PER_SEC*3);}}//ι��ר��
void Task1(void *ppdata) reentrant{//��������
	//ppdata = ppdata;
	ET0=1;
	for(;;){
		TankGame_Input(OSTimeGet());
		OSTimeDly(10);
	}
}

void Task2(void *ppdata) reentrant{//��Ϸ��Ⱦ����
	//ppdata=ppdata;
	for(;;){
		TankGame_Render();
		OSTimeDly(2);
  }
}
void Task3(void *ppdata) reentrant{//����
	//ppdata=ppdata;
	for(;;){
		LED1=~LED1;//��� 0��1��
		OSTimeDly(100);
	}
}
//Game game;

void main(void){
	OSInit();
	//��ʼ����ʱ��
	TMOD &= 0xF0;TMOD |= 0x01;TR0 = 1;
	//оƬIO�ڳ�ʼ��
	P0M0 = 0X00;P0M1 = 0X00;P1M0 = 0X00;P1M1 = 0X00;P2M0 = 0X00;P2M1 = 0X00;P3M0 = 0X00;P3M1 = 0X00;P4M0 = 0X00;P4M1 = 0X00;
	
	TankGame_Init();//��Ϸ��ʼ��
	OSTaskCreate(Task1,(void*)0,&Task1Stk[0],0);//����
	OSTaskCreate(Task2,(void*)0,&Task2Stk[0],1);//��Ⱦ
	OSTaskCreate(Task3,(void*)0,&Task3Stk[0],2);//����ʹ��
	OSTaskCreate(Task_Feed,(void*)0,&TaskFeedStk[0],3);//ι�� ���ȼ�����ԽСԽ����
	OSStart();
}