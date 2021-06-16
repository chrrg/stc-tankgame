#define MaxTankBullet 20
typedef struct{
	GameObject* gameObject;
	uchar status;//1������ 2������ 3������ 4������ 0����
	unsigned long bullet_time;//�ӵ�����ʱ��
} Enemy_Tank;
typedef struct{
	uchar status;//0x00;��λ 1������ 2������ 3������ 4������ ��λ 0�������ӵ� 1����з��ӵ�
	uchar x;
	uchar y;
} Bullet_Data;

void TankGame_Init();
void TankGame_Render();
void TankGame_Input(unsigned long curTime) reentrant;
GameCore* getGame();

