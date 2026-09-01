#include "Enemy.h"
#include "time.h"
#include"Stage.h"
#include"Player.h"
namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 20 * ENEMY_SIZE, 10 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
	const int PLAYER_DISCOVER_DIS = 4;
	Stage* stage = nullptr;
	const int ATTACK_DIS = 2;
	const int Dir[2] = { 1,-1 };
}


Enemy::Enemy()
	: GameObject() 
{
	st = Patrol;
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
	stage=FindGameObject<Stage>();
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	Player* pl = FindGameObject<Player>();
	
	Point ppos = pl->GetPlayerPos();
	float font = GetFontSize();
	int disX = (ppos.x - pos_.x) / CHA_SIZE;
	int disY = (ppos.y - pos_.y) / CHA_SIZE;
	bool PisDiscover = false;
	bool CisDiscover = false;
	bool SisDiscover = false;
	static float prog_timer = 0.5f;
	prog_timer -= Time::DeltaTime();
	Point newPos = pos_;
	if (prog_timer >= 0.0f)
	{
		return;
	}
	switch (st) {
	case Patrol:
		// 通常移動
		PisDiscover = abs(disX) < PLAYER_DISCOVER_DIS && abs(disY) < PLAYER_DISCOVER_DIS;
		switch (dir_)
		{
		case UP:
			newPos.y -= ENEMY_DRAW_SIZE;
			break;

		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			break;

		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			break;
		}
		if (PisDiscover)
		{
			st = Chase;
		}
		break;

	case Chase:
		CisDiscover = abs(disX) < PLAYER_DISCOVER_DIS && abs(disY) < PLAYER_DISCOVER_DIS;
		if (abs(disX) > abs(disY))
		{
			if (disX > 0)
			{
				newPos.x += ENEMY_DRAW_SIZE;
				dir_ = RIGHT;
			}
			else if (disX < 0)
			{
				newPos.x -= ENEMY_DRAW_SIZE;
				dir_ = LEFT;
			}
		}
		else
		{
			if (disY > 0)
			{
				newPos.y += ENEMY_DRAW_SIZE;
				dir_ = DOWN;
			}
			else if (disY < 0)
			{
				newPos.y -= ENEMY_DRAW_SIZE;
				dir_ = UP;
			}
		}
		if (!CisDiscover) {
			st = Patrol;
		}
		if (abs(disX) < ATTACK_DIS && abs(disY) < ATTACK_DIS) {
			st = Attack;
		}
		break;
	case Attack:
	
		SetFontSize(font*3);
		DrawString(0,798, "攻撃中", GetColor(255, 0, 0));
		if (disX > ATTACK_DIS || disY > ATTACK_DIS) {
			st = Search;
		}
		SetFontSize(font);
		break;
	case Search:
		for (int i = 0;i < 4;i++) {
			int disX = (ppos.x - pos_.x) / CHA_SIZE;
			int disY = (ppos.y - pos_.y) / CHA_SIZE;
			SisDiscover = abs(disX+Dir[i%2]) < PLAYER_DISCOVER_DIS && abs(disY+Dir[i%2]) < PLAYER_DISCOVER_DIS;
			if (SisDiscover) {
				st = Attack;
				break;
			}
		}
		if (!SisDiscover) {
			st = Patrol;
		}
		break;
	}
	// 壁じゃなければ移動
	if (stage->GetMapData(newPos.x / ENEMY_DRAW_SIZE, newPos.y / ENEMY_DRAW_SIZE) != 1)
	{
		pos_ = newPos;
	}
	else
	{
		// 壁だったら方向転換
		switch (dir_) {
		case UP:
			dir_ = LEFT;
			break;
		case DOWN:
			dir_ = RIGHT;
			break;
		case RIGHT:
			dir_ = UP;
			break;
		case LEFT:
			dir_ = DOWN;
			break;
		}
	}
	prog_timer += 0.5f;
}
void Enemy::Draw()
{
	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];
	Point frontVec;//向きベクトル
	switch (dir_) {
		case UP:
		frontVec = { 0, -1 };
		break;
		case DOWN:
		frontVec = { 0, 1 };
		break;
		case LEFT:
		frontVec = { -1, 0 };
		break;
		case RIGHT:
		frontVec = { 1,0 };
		break;
	}
	float angle = DX_PI_F / 3.0f;
	float lx =frontVec.x * cosf(angle)- frontVec.y * sinf(angle);

	float ly =frontVec.x * sinf(angle)+ frontVec.y * cosf(angle);

	float rx= frontVec.x * cosf(-angle)- frontVec.y * sinf(-angle);

	float ry=frontVec.x* sinf(-angle)+ frontVec.y * cosf(-angle);
	int range = PLAYER_DISCOVER_DIS * ENEMY_DRAW_SIZE;
	int centerX = pos_.x + ENEMY_SIZE / 2;
	int centerY = pos_.y + ENEMY_SIZE / 2;
	int leftX = centerX + lx * range;
	int leftY = centerY + ly * range;

	int rightX = centerX + rx * range;
	int rightY = centerY + ry * range;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

	DrawTriangle(
		centerX,
		centerY,
		leftX,
		leftY,
		rightX,
		rightY,
		GetColor(255, 0, 0),
		TRUE
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE, 2);
	DrawRectExtendGraph(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();
	int topX= pos_.x-PLAYER_DISCOVER_DIS * ENEMY_DRAW_SIZE;
	int topY = pos_.y-PLAYER_DISCOVER_DIS * ENEMY_DRAW_SIZE;
	int bottomX = pos_.x+PLAYER_DISCOVER_DIS * ENEMY_DRAW_SIZE;
	int bottomY = pos_.y+PLAYER_DISCOVER_DIS * ENEMY_DRAW_SIZE;
	//// 透明度を50%（128/255）に設定
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//// 赤色の半透明矩形を描画（引数: 左上X, 左上Y, 右下X, 右下Y, 色, 塗りつぶし）
	//DrawBox(topX, topY, bottomX + ENEMY_DRAW_SIZE, bottomY + ENEMY_DRAW_SIZE, GetColor(255, 0, 0), TRUE);
	//// 描画が終わったら通常のブレンドモードに戻す
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
}
