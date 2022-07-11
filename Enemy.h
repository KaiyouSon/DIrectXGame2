#pragma once
#include <cstdint>
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include "BaseEnemyState.h"

class Enemy
{
public:
	enum class Phase
	{
		Approach,	// �ڋ߂���
		Leave		// ���E����
	};

private:
	uint32_t textureHandle = 0;
	Model* model = nullptr;
	WorldTransform trans;
	Vector3 pos;
	float speed;
	Phase phase = Phase::Approach;

	// ���
	BaseEnemyState* currentState;
public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw();

	void SetPos(Vector3 pos);
	Vector3 GetPos();
};
