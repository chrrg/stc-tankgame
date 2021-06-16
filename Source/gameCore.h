/*
CH GameCore
Version: 1.0
Author: CH
Date: 2020-07-01
*/

#include <stdlib.h>
#define MAX_GAMEOBJECT_COUNT 6//游戏场景最多对象个数限制
typedef unsigned int uint;
typedef unsigned char uchar;
//typedef unsigned char bool;
#define TRUE 1
#define FALSE 0
typedef enum {false , true } boolean;
//enum boolean { false,true };


typedef struct{//每一个游戏纹理
	uchar* pic;
	uchar w;
	uchar h;
} GamePic;
struct GameObjectTypeObject{
	uchar x;
	uchar y;
	GamePic* pic;//一个游戏对象只能绑定一个纹理 对象宽高在这里
	boolean isDisplay;//0时不渲染对象 非0即渲染
	uchar id;//id为无符号整数
	struct GameObjectTypeObject* children[MAX_GAMEOBJECT_COUNT];//游戏对象的子对象列表
};
typedef struct GameObjectTypeObject GameObject;

typedef struct{
	uchar screen_width;//屏幕宽
	uchar screen_height;//屏幕高
	GameObject *rootGameObject;//一个游戏只能有一个根对象
	void (*beforeDrawCallback) () reentrant;//渲染前执行
	void (*afterDrawCallback) () reentrant;//渲染后执行
	uchar fps;
	uchar fpsCount;
	unsigned long fpsTime;
	uchar *bitmap;//画布像素数组
	uchar *buffer;//缓冲 修改缓冲
	boolean render_flag;//渲染标志 是否已渲染
	unsigned long time;//现在时间
} GameCore;

void initGameCore();//初始化游戏引擎
GameCore* newGameCore(uint w,uint h);//实例化一个新的游戏引擎
void DestroyGameCore(GameCore* game);//销毁游戏引擎
void setRootGameObject(GameCore* game,GameObject* gameObject);//设置根游戏对象
GameObject* newGameObject();//实例化一个新的游戏对象
void FreeGameObject(GameObject* gameObject);//销毁游戏对象
void setGameObjectPos(GameObject* gameObject,uint x,uint y) reentrant;//设置游戏对象的位置 X和Y 相对屏幕左上角为0
void setGameObjectBitmap();//设置游戏对象的纹理 同时设置对象宽高
void doGameRender(GameCore* game);//执行渲染程序
//void doCallback(GameCore* game);//回调
GamePic* newGamePic(unsigned char* pic,unsigned int w,unsigned int h);
void setGamePicOnGameObject(GamePic* pic,GameObject* obj) reentrant;
void setDisplay(GameObject* gameObject,boolean is);
//boolean addCallBack(GameCore* game,void (*fn)());
//boolean removeCallBack(GameCore* game,void (*fn)());
boolean appendChild(GameObject* parent,GameObject *child);
boolean removeChild(GameObject* parent,GameObject *child);
void drawNumber(GameCore *game,unsigned long number,uint x,uint y) reentrant;
void doGameFullRender(GameCore* gameCore);
boolean getPixelFromGame(GameCore* game,uint x,uint y);
void* mem_calloc(uint size);
void drawPixel(GameCore* game,uchar x,uchar y,boolean result);