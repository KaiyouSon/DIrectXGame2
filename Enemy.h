#pragma once
#include <cstdint>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
class Enemy
{
private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;
	Vector3 pos;
	float speed;
public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw();
};