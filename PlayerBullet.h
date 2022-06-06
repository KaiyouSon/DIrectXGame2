#pragma once
#include <cstdint>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
class PlayerBullet
{
private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;
	Vector3 pos;
	Vector3 vec;
	int aliveTimer;
	float speed;
	bool isShot;

public:
	PlayerBullet();
	~PlayerBullet();
	void Initialize();
	void Generate(const Vector3& pos, const Vector3& vec);
	void Update();
	void Draw();

	bool GetisShot();
};

