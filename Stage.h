#pragma once
#include "./Library/GameObject.h"
#include "Source/Screen.h"
class Stage :
    public GameObject
{
public:
	Stage();
	~Stage();
	void Update() override;
	void Draw() override;
	int GetMapData(int x, int y);
};

