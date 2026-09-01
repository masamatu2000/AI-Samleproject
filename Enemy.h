#pragma once
#include ".\Library\GameObject.h"
enum State {
	Patrol,//„‰ñ(‹ŠE‚É“ü‚Á‚½‚ç‹——£‹l‚ß‚é)
	Chase,//‹——£‹l‚ß‚éiUŒ‚‚Å‚«‚é‹——£‚É“ü‚Á‚½‚çUŒ‚j
	Attack,//UŒ‚i‚ ‚é’ö“xUŒ‚‹——£‚©‚ç“¦‚°‚ç‚ê‚½‚ç’T‚·j
	Search,//’T‚·(‚¤‚ë‚¤‚ë‚µ‚ÄŒ©‚Â‚©‚ç‚È‚©‚Á‚½‚çƒpƒgƒ[ƒ‹ Ä‚Ñ‹ŠE‚É“ü‚Á‚½‚çAttack)
};
class Enemy;
class EnemyState
{
public:
	virtual ~EnemyState() = default;
	virtual void Update(Enemy* enemy) = 0;
};
class EnemyPatrolState : public EnemyState
{
public:
	void Update(Enemy* enemy) override;
};
class EnemyChaseState : public EnemyState
{
public:
	void Update(Enemy* enemy) override;
};
class EnemyAttackState : public EnemyState
{
public:
	void Update(Enemy* enemy) override;
};
class EnemySearchState : public EnemyState
{
public:
	void Update(Enemy* enemy) override;

};
class Enemy :
    public GameObject
{
	int hImage_;//‰æ‘œID
	Point pos_;//ˆÊ’u
	DIR dir_;//ˆÚ“®•ûŒü
	State currentState_;//Œ»İ‚Ìó‘Ô
	EnemyState* state_;//ó‘Ô‚Ìƒ|ƒCƒ“ƒ^
	bool ChangeStateFlag_ = false;
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
	Point GetPos() const { return pos_; }
	DIR GetDir() const { return dir_; }
	void SetDir(DIR dir) { dir_ = dir; }
	void SetPos(Point pos) { pos_ = pos; }
	State st;
	void ChangeState(State newState);
};
