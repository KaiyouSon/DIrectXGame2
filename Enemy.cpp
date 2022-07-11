#include "Enemy.h"
#include "Util.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

Enemy::Enemy() :
	generatePos(15, 0, 30), pos(generatePos),
	speed(0.25), currentState(new EnemyStateApproach),
	enemyBullet(new EnemyBullet)
{
}

Enemy::~Enemy()
{
	delete enemyBullet;
	delete model;
	delete currentState;
}

void Enemy::Initialize()
{
	textureHandle = TextureManager::Load("kuribo.png");
	model = Model::Create();

	currentState->Initialize();
	enemyBullet->Initialize();

	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}

void Enemy::Update()
{
	currentState->Update();
	if (currentState->GetisEnd() == true)
	{
		BaseEnemyState* tmp = currentState->NextState();
		tmp->Initialize();
		delete currentState;
		currentState = tmp;
	}


	if (enemyBullet->GetisActive() == false)
		enemyBullet->Generate(pos);

	enemyBullet->Update();

	trans.translation_ = pos;
	trans.WorldTransformationMatrix();
}

void Enemy::Draw()
{
	enemyBullet->Draw();
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