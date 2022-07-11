#include "Enemy.h"
#include "Util.h"
#include "EnemyStateApproach.h"
#include "EnemyStateLeave.h"

Enemy::Enemy() :
	pos(0, 0, 30), speed(0.25), currentState(new EnemyStateApproach)

{

}

Enemy::~Enemy()
{
	delete currentState;
}

void Enemy::Initialize()
{
	textureHandle = TextureManager::Load("kuribo.png");
	model = Model::Create();

	currentState->Initialize();

	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}

void Enemy::Update()
{
	//(this->*spFuncTable[static_cast<size_t> (phase)])();

	currentState->Update();
	if (currentState->GetisEnd() == true)
	{
		BaseEnemyState* tmp = currentState->NextState();
		tmp->Initialize();
		delete currentState;
		currentState = tmp;
	}

	trans.translation_ = pos;
	trans.WorldTransformationMatrix();
}

void Enemy::Draw()
{
	model->Draw(trans, view, textureHandle);
}

void Enemy::SetPos(Vector3 pos)
{
	this->pos = pos;
}

Vector3 Enemy::GetPos()
{
	return pos;
}