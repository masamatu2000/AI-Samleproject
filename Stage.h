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
	void SetMapData(int x, int y,int value) {
		//mapData[y][x] = value;
	}
};

