/*
CH GameCore
Version: 1.0
Author: CH
Date: 2020-07-01
*/
#include "gameCore.h"
#include "stdlib.h"
#include "user\IAP15W4K61S4.h"
/************* 12864LCD引脚定义 *************/
#define LCD_data  P0       //数据口
sbit LCD_RS  =  P3^5;      //寄存器选择输入 
sbit LCD_RW  =  P3^6;      //液晶读/写控制
sbit LCD_EN  =  P3^7;      //液晶使能控制
sbit LCD_PSB =  P4^6;      //串/并方式控制
sbit LCD_RST =  P4^7;      //液晶复位端口
void _nop_ (void);
uchar code mask1[]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};//10000000 01000000 ~ 00000001
uchar code mask2[]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE};//01111111 10111111 11011111
uchar code mask3[]={0,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};//10000000 11000000 ~ 11111110
uchar code mask4[]={0,0x1,0x3,0x7,0xF,0x1F,0x3F,0x7F};//00000001 00000011 ~ 01111111
uchar code number58[]={//5*8 0123456789数字
	0x31,0x0C,0x61,0x3C,0xCF,0x31,0x80,0x4B,0x12,0x93,0x21,0x29,0x4A,0x40,0x49,0x02,
	0x13,0x21,0x01,0x4A,0x40,0x49,0x04,0x65,0x39,0xC2,0x31,0xC0,0x49,0x08,0x15,0x05,
	0x24,0x48,0x40,0x49,0x10,0x97,0x85,0x24,0x4A,0x40,0x33,0x9E,0x61,0x38,0xC4,0x31,
	0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static unsigned char xdata gameCore_membuff[3100];//3KB内存
sbit LED2 = P4^2;
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};
bit lcd_busy(){
	bit result;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	delayNOP();
	result = (bit)(P0&0x80);
	LCD_EN = 0;
	return result; 
}
/*******************************************************************/
/*                                                                 */
/*写指令数据到LCD                                                  */
/*RS=L，RW=L，E=高脉冲，D0-D7=指令码。                             */
/*                                                                 */
/*******************************************************************/
void lcd_wcmd(uchar cmd){
	while(lcd_busy());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	_nop_();
	_nop_(); 
	P0 = cmd;
	delayNOP();
	LCD_EN = 1;
	delayNOP();
	LCD_EN = 0;  
}
/*******************************************************************/
/*                                                                 */
/*写显示数据到LCD                                                  */
/*RS=H，RW=L，E=高脉冲，D0-D7=数据。                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(uchar dat){
	while(lcd_busy());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	P0 = dat;
	delayNOP();
	LCD_EN = 1;
	delayNOP();
	LCD_EN = 0; 
}
/*******************************************************************/
/*                                                                 */
/*  LCD初始化设定                                                  */
/*                                                                 */
/*******************************************************************/
void delay_fast(unsigned int ii){
	while(ii-->0)_nop_();
}
void lcd_init(){
	LCD_PSB = 1;         //并口方式
	LCD_RST = 0;		 //液晶复位
	LCD_RST = 1;      
	lcd_wcmd(0x34);      //扩充指令操作
	lcd_wcmd(0x30);      //基本指令操作
	lcd_wcmd(0x0C);      //显示开，关光标
	lcd_wcmd(0x01);      //清除LCD的显示内容
}

void lcd_Clear(){//清屏 
	lcd_wcmd(0x34);
	delay_fast(15);
	lcd_wcmd(0x30);
	delay_fast(15);
	lcd_wcmd(0x01);
	delay_fast(15);
}
//////////////////////////////////////////////////12864驱动结束//////////////////////////////////////////
void initGameCore(){
	init_mempool(gameCore_membuff, sizeof(gameCore_membuff));
	lcd_init();//显示屏初始化
}
GamePic* newGamePic(unsigned char* pic,unsigned int w,unsigned int h){
	GamePic *gamePic;
	gamePic = (GamePic *)calloc(1,sizeof(GamePic));//使用calloc会初始化为0
	gamePic->pic=pic;
	gamePic->w=w;
	gamePic->h=h;
	return gamePic;
}
void clsGameRender(){
	uint i,j;
	//lcd_wcmd(0x34);
	for(i=0;i<32;i++){
    lcd_wcmd(0x80+i);lcd_wcmd(0x80);
    for(j=0;j<16;j++)lcd_wdat(0);
		delay_fast(10);
  }
  for(i=0;i<32;i++){ 
    lcd_wcmd(0x80+i);lcd_wcmd(0x88);
    for(j=0;j<16;j++)lcd_wdat(0);    
		delay_fast(10);
  }
  //lcd_wcmd(0x36);
}
GameCore* newGameCore(uint w,uint h){//初始化游戏内核，保证数据初使化
	GameCore* gameCore=(GameCore *)calloc(1,sizeof(GameCore));//使用calloc会初始化为0
	//下面开始初始化游戏引擎
	gameCore->screen_width=w;//w=128;
	gameCore->screen_height=h;//h=64;
	gameCore->bitmap=(uchar *)calloc(1,w*h/8);//修改这个
	gameCore->buffer=(uchar *)calloc(1,w*h/8);//这个是一次性画上去
	lcd_Clear();//向12864发送清屏指令
	return gameCore;
}
//void doCallback(GameCore* gameCore){
	//int i=0;
	//if(gameCore==NULL)return;
	
	/*do{
		if(gameCore->Callback[i]!=NULL)gameCore->Callback[i]();
	}while(++i<MAX_CALLBACK_COUNT);*/
//}
void drawPixel(GameCore* game,uchar x,uchar y,boolean result){
	if(x<0||y<0||x>=game->screen_width||y>=game->screen_height)return;
	if(result){
		game->bitmap[game->screen_width/8*y+x/8]|=mask1[x%8];
	}else
		game->bitmap[game->screen_width/8*y+x/8]&=mask2[x%8];
}
void drawByte(GameCore* game,uchar x,uchar y,uchar result){//这里的x是byte坐标
	game->bitmap[game->screen_width/8*y+x]=result;
}
/*
void drawByteLeft(GameCore* game,uchar x,uchar y,uchar result,int n){//11100000
	//将result的前n位写入game中
	game->bitmap[game->screen_width/8*y+x]&=mask4[8-n];//取原来的后8-n位
	game->bitmap[game->screen_width/8*y+x]&=result&mask3[n];//取新的前n位
}
void drawByteRight(GameCore* game,uchar x,uchar y,uchar result,int n){//00000111
	game->bitmap[game->screen_width/8*y+x]&=mask3[8-n];//取原来的前8-n位
	game->bitmap[game->screen_width/8*y+x]&=result&mask4[n];//取后n位合并
}
void drawByteLeftRight(GameCore* game,uchar x,uchar y,uchar result,int n,int n2){//0001 1100 n=3 n2=2
	game->bitmap[game->screen_width/8*y+x]&=mask3[n]|mask4[n2]&mask4[8-n]&mask3[8-n2]&result;
}
*/
boolean getPixel(GamePic* gamePic,uint x,uint y){
	int w;
	w=gamePic->w;
	if(w%8!=0)w+=8-w%8;
	return (gamePic->pic[w*y/8+x/8]&mask1[x%8])>>(7-x%8);
}
uchar getByte(GamePic* gamePic,uint x,uint y){//这里x是byte坐标
	int w;
	w=gamePic->w;//12
	if(w%8!=0){w+=8-w%8;}
	return gamePic->pic[w/8*y+x];
}
boolean getPixelFromGame(GameCore* game,uint x,uint y){
	return (game->buffer[game->screen_width/8*y+x/8]&mask1[x%8])>>(7-x%8);
}
void doDrawObject(GameCore* game,GameObject* gameObject){//画一个对象
	int i,j,x,y,w,h;
	if(gameObject->pic==NULL)return;
	x=gameObject->x;//待渲染对象的X像素坐标横轴
	y=gameObject->y;//待渲染对象的Y像素坐标纵轴
	w=gameObject->pic->w;//待渲染的对象宽像素
	h=gameObject->pic->h;//待渲染的对象高像素
	
	if(x%8==0&&w%8==0){//快速渲染组件
		//if(w%8==0){
			for(j=0;j<h;j++)//纵轴
				for(i=0;i<w/8;i++)//横轴
					drawByte(game,x/8+i,j+y,getByte(gameObject->pic,i,j));
		/*}else{
			for(j=0;j<h;j++){//纵轴
				for(i=0;i<w/8;i++)//横轴
					drawByte(game,x/8+i,j+y,getByte(gameObject->pic,i,j));
				i=0;
				drawByteLeft(game,x/8+i,j+y,getByte(gameObject->pic,i,j),w%8);
			}
		}*/
	}else{
		
		/*if(w%8==0){
			for(j=0;j<h;j++){//纵轴
				i=0;
				if(w>=8-x%8){
					drawByteRight(game,i+x/8,j,getByte(gameObject->pic,i,j)>>(x%8),8-x%8);
					for(i=1;i<(w+x%8)/8;i++){
						
						//drawByte(game,x/8+i,j+y,getByte(gameObject->pic,i,j)<<(x%8));
					}
				}else{
					drawByteLeftRight(game,i+x/8,j,getByte(gameObject->pic,i,j)>>(x%8),x%8,8-w-x%8);
				}
			}
		}
		x%8;
		if(w%8==0){
			
		}else{
			
		}*/
		//LED2=~LED2;
		//通用渲染组件 性能低
		
		//if(w%8!=0)w+=8-w%8;
		for(j=0;j<h;j++)//纵轴
			for(i=0;i<w;i++)//横轴
				if(getPixel(gameObject->pic,i,j))drawPixel(game,x+i,y+j,1);else drawPixel(game,x+i,y+j,0);
	}
}
void doDrawView(GameCore* game,GameObject* gameObject) reentrant{//画一层
	int i=0;
	if(gameObject->isDisplay==0)return;
	doDrawObject(game,gameObject);
	do{
		if(gameObject->children[i]==NULL)return;
		doDrawView(game,gameObject->children[i]);
	}while(++i<MAX_GAMEOBJECT_COUNT);
}
void setDisplay(GameObject* gameObject,boolean is){
	gameObject->isDisplay=is;
}
void doDraw(GameCore* gameCore){//渲染一帧
	int i;
	if(gameCore==NULL)return;
	if(gameCore->rootGameObject==NULL)return;
	for(i=0;i<1024;i++)gameCore->bitmap[i]=0;
	if(gameCore->beforeDrawCallback!=NULL)gameCore->beforeDrawCallback();
	doDrawView(gameCore,gameCore->rootGameObject);
	if(gameCore->afterDrawCallback!=NULL)gameCore->afterDrawCallback();
}
void doGameFullRender(GameCore* gameCore){
	uchar *bmp;
  uchar i,j;
	int n=0;
	bmp=gameCore->bitmap;
  lcd_wcmd(0x34);
  for(i=0;i<32;i++){
    lcd_wcmd(0x80+i);
    lcd_wcmd(0x80);
    for(j=0;j<16;j++)lcd_wdat(bmp[n++]);
    delay_fast(15);
  }
  for(i=0;i<32;i++)
  { 
    lcd_wcmd(0x80+i);
    lcd_wcmd(0x88);
    for(j=0;j<16;j++)lcd_wdat(bmp[n++]);
		delay_fast(15);
  }
  lcd_wcmd(0x36);
}
void doGameRender(GameCore* gameCore){//将缓冲区发送到屏幕
	int i,j,w,h,n=0;
	boolean isUpdate=false;//是否有更新
	if(gameCore==NULL)return;
	if(gameCore->fpsTime+100<gameCore->time){
		gameCore->fpsTime=gameCore->time;
		gameCore->fps=gameCore->fpsCount;
		gameCore->fpsCount=0;
	}
	gameCore->fpsCount++;
	doDraw(gameCore);
	//doGameFullRender(gameCore);return;//完全渲染
	
	w=(int)gameCore->screen_width /4;//  128/4=32
	h=(int)gameCore->screen_height/4;//  64 /4=16
	
	for(i=0;i<w;i++){//0~32
		for(j=0;j<h;j+=2){//0~16
			n=i*16+j;
			if(gameCore->render_flag==0||gameCore->buffer[n]!=gameCore->bitmap[n] || gameCore->buffer[n+1]!=gameCore->bitmap[n+1]){
				if(!isUpdate){lcd_wcmd(0x34);isUpdate=true;delay_fast(15);}
				lcd_wcmd(0x80+i);//先写入水平坐标值	
				lcd_wcmd(0x80+j/2);//写入垂直坐标值
				lcd_wdat(gameCore->buffer[n]=gameCore->bitmap[n]);
				lcd_wdat(gameCore->buffer[n+1]=gameCore->bitmap[n+1]);
				delay_fast(15);
			}
		}//0~16 一行像素绘制完成
	}//上半屏
	for(i=0;i<w;i++){//0~32
		for(j=0;j<h;j+=2){
			n=512+i*16+j;
			if(gameCore->render_flag==0||gameCore->buffer[n]!=gameCore->bitmap[n]||gameCore->buffer[n+1]!=gameCore->bitmap[n+1]){
				if(!isUpdate){lcd_wcmd(0x34);isUpdate=true;delay_fast(15);}
				lcd_wcmd(0x80+i);//先写入水平坐标值
				lcd_wcmd(0x88+j/2);//写入垂直坐标值
				lcd_wdat(gameCore->buffer[n]=gameCore->bitmap[n]);
				lcd_wdat(gameCore->buffer[n+1]=gameCore->bitmap[n+1]);
				delay_fast(15);
			}
		}//0~16 一行像素绘制完成
	}//下半屏
	if(isUpdate){lcd_wcmd(0x36);}//写完数据,开图形显示
	gameCore->render_flag=1;
	
}
void FreeGameCore(GameCore* game){//释放游戏对象 非释放所有内存
	free(game);
	game=NULL;
}
void DestroyGameCore(GameCore* game){//销毁游戏对象 释放游戏所有对象并释放所有内存
	game=game;
	//free(game);
	//game=NULL;
}
GameObject* findElementByIdFromGameObject(GameObject* gameObject,uchar id) reentrant{
	int i=0;
	if(gameObject->id==id)return gameObject;
	do{
		if(gameObject->children[i]!=NULL){
			GameObject* result;
			result=findElementByIdFromGameObject(gameObject->children[i],id);
			if(result!=NULL)return result;
		}
	}while(++i<MAX_GAMEOBJECT_COUNT);
	return NULL;
}
GameObject* findElementById(GameCore* game,uchar id){//从游戏中找到指定id的元素
	return findElementByIdFromGameObject(game->rootGameObject,id);
}
GameObject* newGameObject(){
	GameObject *gameObject = (GameObject *)calloc(1,sizeof(GameObject));//使用calloc会初始化为0 malloc则不会
	gameObject->isDisplay=true;//默认显示
	return gameObject;
}
void setGameObjectPos(GameObject* gameObject,uint x,uint y) reentrant{//设置对象位置
	gameObject->x=x;
	gameObject->y=y;
}
void setGameObjectSize(){
	//不允许设置游戏对象大小 只能通过设置纹理来改变游戏大小
}
void setRootGameObject(GameCore* game,GameObject* gameObject){//设置根游戏对象
	game->rootGameObject=gameObject;
}
void setGameObjectBitmap(GameObject* gameObject,GamePic *gamePic){//设置对象纹理（图像）
	gameObject->pic=gamePic;
}
void FreeGameObject(GameObject* gameObject){
	free(gameObject);
	gameObject=NULL;
}
void setGamePicOnGameObject(GamePic* pic,GameObject* obj) reentrant{
	//if(obj->pic!=NULL)free(obj->pic);
	obj->pic=pic;//指向新的纹理
}
boolean appendChild(GameObject* parent,GameObject *child){//添加一个游戏对象到游戏中
	int i=0;
	do{
		if(parent->children[i]==NULL){
			parent->children[i]=child;
			return true;
		}
	}while(++i<MAX_GAMEOBJECT_COUNT);
	return false;
}
boolean removeChild(GameObject* parent,GameObject *child){//从游戏中删除一个游戏对象
	int i=0;
	do{
		if(parent->children[i]==child){
			parent->children[i]=NULL;
			return true;
		}
	}while(++i<MAX_GAMEOBJECT_COUNT);
	return false;
}
void drawOneNumber(GameCore *game,unsigned char number,uint x,uint y){
	int i,j,n,offset;
	if(number>9||number<0)return;
	//n=number*5/8;
	//offset=(number*5)%8;
	for(j=0;j<8;j++){//列
		for(i=0;i<5;i++){//行
			n=(number*5+i)/8+j*7;
			offset=(number*5+i)%8;
			if((number58[n]&mask1[offset])>>(7-offset))
				drawPixel(game,x+i,y+j,1);
			else
				drawPixel(game,x+i,y+j,0);
			/*if((number58[n]&mask1[offset+i+j*5])>>(7-offset))
				drawPixel(game,x+i,y+j,1);
			else
				drawPixel(game,x+i,y+j,0);*/
		}
	}
}
void drawNumber(GameCore *game,unsigned long number,uint x,uint y) reentrant{
	unsigned long n;
	int count=0;
	n=number;
	do{
		n/=10;
		count++;
	}while(n!=0);
	//count位数
	n=number;
	while(count-->0){
		drawOneNumber(game,n%10,x+5*count,y);
		n/=10;
	}
}
void* mem_calloc(uint leng){
	return calloc(1,leng);
}
/*boolean addCallBack(GameCore* game,void (*fn)()){//添加一个回调到游戏中
	int i=0;
	do{
		if(game->Callback[i]==NULL){
			game->Callback[i]=fn;//添加成功
			return true;
		}
	}while(++i<MAX_GAMEOBJECT_COUNT);
	return false;
}
boolean removeCallBack(GameCore* game,void (*fn)()){//移除游戏回调
	int i=0;
	do{
		if(game->Callback[i]==fn){
			game->Callback[i]=NULL;
			return true;
		}
	}while(++i<MAX_GAMEOBJECT_COUNT);
	return false;
}*/