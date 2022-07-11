#include "Enemy.h"
#include "Util.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "DebugText.h"

Enemy::Enemy() :
	generatePos(15, 0, 30), pos(generatePos),
	speed(0.25), currentState(new EnemyStateApproach),
	generateTimer(0), maxGenerateTimer(60)
{
}
Enemy::~Enemy()
{
	delete model;
	delete currentState;
}

void Enemy::Initialize()
{
	textureHandle = TextureManager::Load("kuribo.png");
	model = Model::Create();

	currentState->Initialize();
	//enemyBullet->Initialize();

	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}
void Enemy::Update()
{
	DebugText* debug = DebugText::GetInstance();

	// ’e‚Ìˆ—
	ShotUpdate();

	// ó‘Ô‘JˆÚ
	StateChangeUpdate();

	trans.translation_ = pos;
	trans.WorldTransformationMatrix();

	//debug->SetPos(0, 0);
	//debug->Printf("generateTimer = %f", generateTimer);
}
void Enemy::Draw()
{
	for (std::unique_ptr <EnemyBullet>& enemyBullet : enemyBullets)
	{
		enemyBullet->Draw();
	}
	model->Draw(trans, view, textureHandle);
}

void Enemy::SetPos(Vector3 pos)
{
	this->pos = pos;
}

Vector3 Enemy::GetGeneratePos()
{
	return generatePos;
}
Vector3 Enemy::GetPos()
{
	return pos;
}

void Enemy::ShotUpdate()
{
	if (currentState->GetTag() == "Approach")
	{
		generateTimer++;
		if (generateTimer >= maxGenerateTimer)
		{
			std::unique_ptr <EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize();
			if (newBullet->GetisActive() == false)
				newBullet->Generate(pos);
			enemyBullets.push_back(std::move(newBullet));
			generateTimer = 0;
		}
	}
	else generateTimer = 0;

	for (std::unique_ptr <EnemyBullet>& enemyBullet : enemyBullets)
	{
		enemyBullet->Update();
	}
}
void Enemy::StateChangeUpdate()
{
	currentState->Update();
	if (currentState->GetisEnd() == true)
	{
		BaseEnemyState* tmp = currentState->NextState();
		tmp->Initialize();
		delete currentState;
		currentState = tmp;
	}
}
