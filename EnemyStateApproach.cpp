#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "Enemy.h"

extern Enemy* enemy;

EnemyStateApproach::EnemyStateApproach() :
	speed(0.25)
{
}

void EnemyStateApproach::Initialize()
{
	pos = enemy->GetPos();
	isEnd = false;
}

void EnemyStateApproach::Update()
{
	pos.z -= speed;
	enemy->SetPos(pos);

	if (pos.z <= 0)
	{
		pos = { 0,0,30 };
		isEnd = true;
	}
}

BaseEnemyState* EnemyStateApproach::NextState()
{
	return new EnemyStateLeave;
}
