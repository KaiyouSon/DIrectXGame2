#include "Enemy.h"
#include "Util.h"

Enemy::Enemy() :
	pos(0, 0, 0)
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	textureHandle = TextureManager::Load("kuribo.png");
	model = Model::Create();

	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}

void Enemy::Update()
{
	pos.z -= 0.25;
	if (pos.z <= view.eye.z) pos.z = 30;
	trans.translation_ = pos;
	trans.WorldTransformationMatrix();
}

void Enemy::Draw()
{
	model->Draw(trans, view, textureHandle);
}
