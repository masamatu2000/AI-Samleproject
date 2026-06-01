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
}


Enemy::Enemy()
	: GameObject() 
{
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

	static float prog_timer = 0.5f;
	prog_timer -= Time::DeltaTime();

	if (prog_timer >= 0.0f)
	{
		return;
	}

	int disX = (ppos.x - pos_.x) / CHA_SIZE;
	int disY = (ppos.y - pos_.y) / CHA_SIZE;

	bool isDiscover =
		abs(disX) < PLAYER_DISCOVER_DIS &&
		abs(disY) < PLAYER_DISCOVER_DIS;

	Point newPos = pos_;

	if (isDiscover)
	{
		// プレイヤー追跡
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
	}
	else
	{
		// 通常移動
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
	Point flontVec;//向きベクトル
	Point pflontVec;
	pflontVec.x = pos_.x * cos(60) - pos_.y * sin(60);
	pflontVec.y = pos_.x * sin(60) + pos_.y * cos(60);
	Point mflontVec;
	mflontVec.x = pos_.x * cos(-60) - pos_.y * sin(-60);
	mflontVec.y = pos_.x * sin(-60) + pos_.y * cos(-60);
	switch (dir_) {
		case UP:
		flontVec = { 0, -1 };
		break;
		case DOWN:
		flontVec = { 0, 1 };
		break;
		case LEFT:
		flontVec = { -1, 0 };
		break;
		case RIGHT:
		flontVec = { 1,0 };
		break;
	}
	
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
	// 透明度を50%（128/255）に設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	// 赤色の半透明矩形を描画（引数: 左上X, 左上Y, 右下X, 右下Y, 色, 塗りつぶし）
	DrawBox(topX, topY, bottomX + ENEMY_DRAW_SIZE, bottomY + ENEMY_DRAW_SIZE, GetColor(255, 0, 0), TRUE);
	// 描画が終わったら通常のブレンドモードに戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		
}
