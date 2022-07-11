#include "Enemy.h"
#include "Util.h"

Enemy::Enemy() :
	pos(0, 0, 30), speed(0.25)
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
	(this->*spFuncTable[static_cast<size_t> (phase)])();


	trans.translation_ = pos;
	trans.WorldTransformationMatrix();
}

void Enemy::Draw()
{
	model->Draw(trans, view, textureHandle);
}

void (Enemy::* Enemy::spFuncTable[])() =
{
	&ApproachUpdate,
	&LeaveUpdate,
};
void Enemy::ApproachUpdate()
{
	pos.z -= speed;
	if (pos.z <= 0) phase = Phase::Leave;
}
void Enemy::LeaveUpdate()
{
	pos.x -= speed;
	pos.y += speed;

	if (pos.x <= -35 || pos.y >= 20)
	{
		pos = { 0,0,30 };
		phase = Phase::Approach;
	}
}