#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"

class EnemyStateApproach : public BaseEnemyState
{
private:
	Vector3 pos;
	float speed;

public:
	EnemyStateApproach();
	void Initialize()override;
	void Update()override;
	BaseEnemyState * NextState()override;

};

