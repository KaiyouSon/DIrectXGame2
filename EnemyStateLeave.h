#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"
class EnemyStateLeave : public BaseEnemyState
{
private:
	Vector3 pos;
	float speed;

public:
	EnemyStateLeave();
	void Initialize()override;
	void Update()override;
	BaseEnemyState* NextState()override;
};

