#pragma once
#include <cstdint>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include "PlayerBullet.h"
class Player
{
private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;
	Vector3 pos;

	const int maxBullet;
	PlayerBullet* bullet;
	int shotTimer;
	int maxShotTimer;
	Vector3 frontVec;
	float rotAngle;

public:
	Player();
	~Player();
	void Initialize();
	void Update();
	void Draw();
};

