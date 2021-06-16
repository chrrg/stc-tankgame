/*
CH GameCore
Version: 1.0
Author: CH
Date: 2020-07-01
*/

#include <stdlib.h>
#define MAX_GAMEOBJECT_COUNT 6//��Ϸ�����������������
typedef unsigned int uint;
typedef unsigned char uchar;
//typedef unsigned char bool;
#define TRUE 1
#define FALSE 0
typedef enum {false , true } boolean;
//enum boolean { false,true };


typedef struct{//ÿһ����Ϸ����
	uchar* pic;
	uchar w;
	uchar h;
} GamePic;
struct GameObjectTypeObject{
	uchar x;
	uchar y;
	GamePic* pic;//һ����Ϸ����ֻ�ܰ�һ������ ������������
	boolean isDisplay;//0ʱ����Ⱦ���� ��0����Ⱦ
	uchar id;//idΪ�޷�������
	struct GameObjectTypeObject* children[MAX_GAMEOBJECT_COUNT];//��Ϸ������Ӷ����б�
};
typedef struct GameObjectTypeObject GameObject;

typedef struct{
	uchar screen_width;//��Ļ��
	uchar screen_height;//��Ļ��
	GameObject *rootGameObject;//һ����Ϸֻ����һ��������
	void (*beforeDrawCallback) () reentrant;//��Ⱦǰִ��
	void (*afterDrawCallback) () reentrant;//��Ⱦ��ִ��
	uchar fps;
	uchar fpsCount;
	unsigned long fpsTime;
	uchar *bitmap;//������������
	uchar *buffer;//���� �޸Ļ���
	boolean render_flag;//��Ⱦ��־ �Ƿ�����Ⱦ
	unsigned long time;//����ʱ��
} GameCore;

void initGameCore();//��ʼ����Ϸ����
GameCore* newGameCore(uint w,uint h);//ʵ����һ���µ���Ϸ����
void DestroyGameCore(GameCore* game);//������Ϸ����
void setRootGameObject(GameCore* game,GameObject* gameObject);//���ø���Ϸ����
GameObject* newGameObject();//ʵ����һ���µ���Ϸ����
void FreeGameObject(GameObject* gameObject);//������Ϸ����
void setGameObjectPos(GameObject* gameObject,uint x,uint y) reentrant;//������Ϸ�����λ�� X��Y �����Ļ���Ͻ�Ϊ0
void setGameObjectBitmap();//������Ϸ��������� ͬʱ���ö�����
void doGameRender(GameCore* game);//ִ����Ⱦ����
//void doCallback(GameCore* game);//�ص�
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