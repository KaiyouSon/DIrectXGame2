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
		Approach,	// 接近する
		Leave		// 離脱する
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

	// 関数テーブル
	static void (Enemy::* spFuncTable[])();

public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw();


};
