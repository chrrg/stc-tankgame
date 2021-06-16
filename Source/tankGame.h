#define MaxTankBullet 20
typedef struct{
	GameObject* gameObject;
	uchar status;//1代表上 2代表左 3代表下 4代表右 0不动
	unsigned long bullet_time;//子弹发射时间
} Enemy_Tank;
typedef struct{
	uchar status;//0x00;高位 1代表上 2代表左 3代表下 4代表右 低位 0代表己方子弹 1代表敌方子弹
	uchar x;
	uchar y;
} Bullet_Data;

void TankGame_Init();
void TankGame_Render();
void TankGame_Input(unsigned long curTime) reentrant;
GameCore* getGame();

