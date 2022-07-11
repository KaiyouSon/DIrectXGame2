#include "EnemyStateLeave.h"
#include "EnemyStateApproach.h"
#include "Enemy.h"

extern Enemy* enemy;

EnemyStateLeave::EnemyStateLeave() :
	speed(0.25)
{
}

void EnemyStateLeave::Initialize()
{
	pos = enemy->GetPos();
	isEnd = false;
}

void EnemyStateLeave::Update()
{
	pos.x -= speed;
	pos.y += speed;
	enemy->SetPos(pos);

	if (pos.x <= -35 || pos.y >= 25)
	{
		pos = enemy->GetGeneratePos();
		enemy->SetPos(pos);
		isEnd = true;
	}
}

BaseEnemyState* EnemyStateLeave::NextState()
{
	return new EnemyStateApproach;
}