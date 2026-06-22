#pragma once
#include ".\Library\GameObject.h"
enum State {
	Patrol,//„‰ñ(‹ŠE‚É“ü‚Á‚½‚ç‹——£‹l‚ß‚é)
	Chase,//‹——£‹l‚ß‚éiUŒ‚‚Å‚«‚é‹——£‚É“ü‚Á‚½‚çUŒ‚j
	Attack,//UŒ‚i‚ ‚é’ö“xUŒ‚‹——£‚©‚ç“¦‚°‚ç‚ê‚½‚ç’T‚·j
	Search,//’T‚·(‚¤‚ë‚¤‚ë‚µ‚ÄŒ©‚Â‚©‚ç‚È‚©‚Á‚½‚çƒpƒgƒ[ƒ‹ Ä‚Ñ‹ŠE‚É“ü‚Á‚½‚çAttack)
};
class Enemy :
    public GameObject
{
	int hImage_;//‰æ‘œID
	Point pos_;//ˆÊ’u
	DIR dir_;//ˆÚ“®•ûŒü
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;
	State st;
};

