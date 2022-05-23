#pragma once
#include <cstdint>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
class Player
{
private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;

	Vector3 pos;

public:
	Player();
	~Player();
	void Initialize();
	void Update();
	void Draw();
};

