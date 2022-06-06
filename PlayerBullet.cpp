#include "PlayerBullet.h"
#include "DebugText.h"
#include "Util.h"
#include "ViewProjection.h"

PlayerBullet::PlayerBullet() :
	pos(0, 0, 0), vec(0, 0, 0),
	speed(2), isShot(false), aliveTimer(0)
{
}

PlayerBullet::~PlayerBullet()
{
	delete model;
}

void PlayerBullet::Initialize()
{
	textureHandle = TextureManager::Load("bullet.png");
	model = Model::Create();

	trans.Initialize();
	trans.translation_ = pos;
	trans.scale_ = { 1,1,1 };
	trans.rotation_ = { 0,0,0 };
	trans.WorldTransformationMatrix();
	isShot = false;
}

void PlayerBullet::Generate(const Vector3& pos, const Vector3& vec)
{
	this->pos = pos;
	this->vec = vec;
	isShot = true;
}

void PlayerBullet::Update()
{
	if (isShot == false) return;
	pos += vec * speed;

	aliveTimer++;
	if (aliveTimer >= 350)
	{
		isShot = false;
		aliveTimer = 0;
	}
	trans.translation_ = pos;
	trans.WorldTransformationMatrix();

}

void PlayerBullet::Draw()
{
	if (isShot == false) return;
	model->Draw(trans, view, textureHandle);
}

bool PlayerBullet::GetisShot()
{
	return isShot;
}
