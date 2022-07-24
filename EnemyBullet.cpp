#include "EnemyBullet.h"
#include "Player.h"
#include "MathUtil.h"
#include "Util.h"
#include <iostream>
extern Player* player;

EnemyBullet::EnemyBullet() :
	pos(0, 0, 0), rot(0, 0, 0), speed(1),
	maxActiveTimer(120), activeTimer(0), isActive(false)
{
}

EnemyBullet::~EnemyBullet()
{
	delete model;
}

void EnemyBullet::Initialize()
{
	textureHandle = TextureManager::Load("enemyBullet.png");
	model = Model::Create();

	trans.translation_ = pos;
	trans.scale_ = { 0.5f,0.5f,3.0f };
	trans.rotation_ = { 0,0,0 };
	trans.Initialize();
	trans.WorldTransformationMatrix();
}

void EnemyBullet::Generate(const Vector3& pos)
{
	this->pos = pos;
	activeTimer = 0;
	isActive = true;
	vec = player->GetPos() - pos;
}

void EnemyBullet::Update()
{
	if (isActive == false) return;

	pos += vec.Normalized() * speed;

	rot.y = atan2(vec.z, vec.x);
	trans.translation_ = pos;
	trans.rotation_ = rot;
	trans.WorldTransformationMatrix();

	activeTimer++;
	if (activeTimer >= maxActiveTimer) isActive = false;
}

void EnemyBullet::Draw()
{
	if (isActive == false) return;

	model->Draw(trans, view, textureHandle);
}
