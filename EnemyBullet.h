#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"

class EnemyBullet
{
private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;
	Vector3 pos;
	float speed;
	float activeTimer;
	float maxActiveTimer;
	bool isActive;

public:
	EnemyBullet();
	~EnemyBullet();
	void Initialize();
	void Generate(const Vector3& pos);
	void Update();
	void Draw();

	bool GetisActive() { return isActive; }
};

