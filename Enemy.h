#pragma once
#include <cstdint>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
class Enemy
{
public:
	enum class Phase
	{
		Approach,	// Ú‹ß‚·‚é
		Leave		// —£’E‚·‚é
	};

private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;
	Vector3 pos;
	float speed;
	Phase phase = Phase::Approach;

	void ApproachUpdate();
	void LeaveUpdate();

	// ŠÖ”ƒe[ƒuƒ‹
	static void (Enemy::* spFuncTable[])();

public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw();


};
