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
		Approach,	// ê⁄ãﬂÇ∑ÇÈ
		Leave		// ó£íEÇ∑ÇÈ
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

public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();
	void Draw();


};
